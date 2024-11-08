//#ifdef _RELEASE

#include "coursework.h"

#include <stdio.h>
#include <string.h>
#include <locale.h>

#include <types.h>
#include <macros.h>
#include "list/list.h"
#include "ref_array/ref_array.h"
#include "interface/interface.h"
#include "compare/compare.h"
#include "file_work/file_work.h"
#include "log/log.h"
#include "undo_stack/undo_stack.h"

static void print_table(WINDOW* winTable, WINDOW* winRed, ProgramInstance* program);

static void change_serialNumber(ProgramInstance* program);
static void change_factoryNumber(ProgramInstance* program);
static void change_directorFullName(ProgramInstance* program);
static void change_engineerFulName(ProgramInstance* program);
static void change_energyConsPlan(ProgramInstance* program);
static void change_energyConsReal(ProgramInstance* program);

static void (* const CHANGE_FUNC_ARRAY[6]) (ProgramInstance* program) = {
    change_serialNumber, change_factoryNumber, change_directorFullName,
    change_engineerFulName, change_energyConsPlan, change_energyConsReal
};

static MenuCommand mainCom[] = {
    {L" Новый список     ", new_list},
    {L" Загрузить список ", load_list},
    //{L"Настройки", settings}
};

static Menu mainMenu = {
    0,
    (SCREEN_HEIGHT / 2 - 5), -7, -1, 1,
    L" Программа ",
    L" Выход     ",
    mainCom,
    sizeof(mainCom) / sizeof(MenuCommand),
    MIDDLE
};

static MenuCommand browsingCom[] = {
    {L" Сохранить  ", save},
    {L" Сортировка ", sorting},
    {L" Найти      ", find},
};

static const Menu browsingMenu = {
    0,
    3, 0, -1, 1,
    L" Редактор ",
    L" Выход    ",
    browsingCom,
    sizeof(browsingCom) / sizeof(MenuCommand),
    LEFT
};


ProgramInstance init_program()
{
    ProgramInstance program;

    // Список и массив вхождений
    program.fecNotes = init_list();
    program.entries = init_ref_array(0);
    program.currentFilePath = "";
    program.undoStack = init_undo();
    program.copyNote = init_note();
    program.copied = false;

    // Работа со списком
    program.selectedNode = 0;
    program.field = 0;
    program.tableMode = FULL;
    program.findMode = false;
    program.chunckSize = CHUNCK_SIZE_FULL;

    program.popUp = newwin(5, 15, 0, 0);
    program.winMain = stdscr;
    program.winTable = newwin(TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH, TABLE_WIN_Y, TABLE_WIN_X),
    program.winRed = newwin(15, 117, 0, 19),
    program.winMenu = newwin(LINES, 19, 0, 0);
    refresh();

    return program;
}

int Main(int argc, char** argv)
{
    setlocale(LC_CTYPE, "");

    // Инициализация PDCurses
    initscr();
    noecho();
    curs_set(0);
    mouse_set(ALL_MOUSE_EVENTS);
    keypad(stdscr, TRUE);
    //timeout(32);
    resize_term(40, 136);
    if (has_colors())
    {
        start_color();
        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(N_INFO, COLOR_BLUE, COLOR_BLACK);
        init_pair(N_ERR, COLOR_RED, COLOR_BLACK);
    }

    ProgramInstance program = init_program();

    Bool shouldClose = false;
    while (!shouldClose)
    {
        //highlight_on_index(&mainMenu, selected);
        print_menu(program.winMain, &mainMenu);
        int ch = getch();

        switch (ch)
        {
        case KEY_UP:
            mainMenu.selected = loop(mainMenu.selected - 1, 0, mainMenu.commandsSize);
            break;
        case KEY_DOWN:
            mainMenu.selected = loop(mainMenu.selected + 1, 0, mainMenu.commandsSize);
            break;
        case '\n':
            if (mainMenu.selected == mainMenu.commandsSize)
            {
                shouldClose = true;
                break;
            }
            mainMenu.commands[mainMenu.selected].function(&program);
            break;
        case KEY_EXIT:
            shouldClose = true;
            break;
        default:
            break;
        }
    }

    endwin();
    return 0;
}

void new_list(ProgramInstance* program)
{
    list_redactor(program);
}

void load_list(ProgramInstance* program)
{
    program->currentFilePath = get_user_input_str(program->popUp, L"Введите название файла:");
    {
        char* filePath = (char*)malloc(strlen(program->currentFilePath) + strlen("input/.txt") + 1);
        if (!filePath)
        {
            LOG_DEBUG(LOG_ERR, "coursework.c", "load_list()", "malloc() returned NULL", LOG_FILE);
            pop_up_notification_wchar(program->popUp, L"Функция malloc() вернула NULL.", N_ERR, POP_UP_Y);
            getch();
            return;
        }
        strcpy(filePath, "input/");
        strcat(filePath, program->currentFilePath);
        strcat(filePath, ".txt");
        DELETE(program->currentFilePath);
        program->currentFilePath = filePath;
    }
    ErrorHandler err = scan_note_list(program->currentFilePath, &program->fecNotes);
    if (err.err != ALL_GOOD)
    {
        char buff[100] = { 0 };
        LOG_DEBUG(LOG_ERR, "coursework.c", "load_list()", proccess_error(buff, err), LOG_FILE);
        print_menu(program->winMain, &mainMenu);
        pop_up_notification_wchar(program->popUp, L"Ошибка чтения файла.", N_ERR, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
        pop_up_notification(program->popUp, proccess_error(buff, err), N_ERR, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 + 3);
        getch();
        return;
    }
    list_redactor(program);
    free(program->currentFilePath);
    program->currentFilePath = "";
}

void list_redactor(ProgramInstance* program)
{
    clear();
    keypad(program->winTable, TRUE);
    keypad(program->winMenu, TRUE);
    refresh();

    

    int chunckSize = CHUNCK_SIZE_FULL;
    while (1)
    {
        print_menu(program->winMenu, &browsingMenu);
        print_table(program->winTable, program->winRed, program);
        /*if (program->undoStack.cur)
        {
            mvprintw(0, 0, "end=%-5d", program->undoStack.end);
            mvprintw(1, 0, "cur:");
            mvprintw(2, 0, "index=%-5d", program->undoStack.cur->index);
            mvprintw(3, 0, "action=%-5d", program->undoStack.cur->action);
            mvprintw(4, 0, "next=%-5p", program->undoStack.cur->next);
            mvprintw(5, 0, "prev=%-5p", program->undoStack.cur->prev);
            refresh();
        }*/

        int ch = getch();
        switch (ch)
        {
        case '\n':
            if (program->tableMode == REDACTOR && program->selectedNode != program->fecNotes.size)
            {
                push_action(&program->undoStack, UNDO_CHANGE, get_at(&program->fecNotes, program->selectedNode), program->selectedNode);
                CHANGE_FUNC_ARRAY[program->field](program);
            }
            else
            {
                program->tableMode = REDACTOR;
                chunckSize = CHUNCK_SIZE_REDACTOR;
            }
            if (program->selectedNode == program->fecNotes.size)
            {
                FECNote note = init_note();
                push_action(&program->undoStack, UNDO_ADD, &note, program->selectedNode);
                insert(&program->fecNotes, &note, program->selectedNode);
                program->tableMode = REDACTOR;
            }
            break;
        case MY_KEY_ESCAPE:
            if (program->tableMode == REDACTOR)
            {
                program->tableMode = FULL;
                chunckSize = CHUNCK_SIZE_FULL;
            }
            break;
        case MY_KEY_CTRL_C:
            if (program->fecNotes.size)
            {
                program->copyNote = *get_at(&program->fecNotes, program->selectedNode);
                program->copied = true;
            }
            break;
        case MY_KEY_CTRL_V:
            if (program->copied && program->fecNotes.size)
            {
                push_action(&program->undoStack, UNDO_ADD, get_at(&program->fecNotes, program->selectedNode), program->selectedNode);
                insert(&program->fecNotes, &program->copyNote, program->selectedNode);
            }
            break;
        case MY_KEY_CTRL_D:
            if (program->fecNotes.size)
            {
                push_action(&program->undoStack, UNDO_DEL, get_at(&program->fecNotes, program->selectedNode), program->selectedNode);
                pop(&program->fecNotes, program->selectedNode);
            }
            break;
        case MY_KEY_CTRL_A:
        {
            FECNote note = init_note();
            push_action(&program->undoStack, UNDO_ADD, &note, program->selectedNode);
            insert(&program->fecNotes, &note, program->selectedNode);
            program->tableMode = REDACTOR;
            break;
        }
        case KEY_LEFT:
            if (program->findMode)
                program->selectedNode = clamp(program->selectedNode - ((program->tableMode == FULL) ? (chunckSize) : (1)), 0, (int)program->entries.size - 1);
            else
                program->selectedNode = clamp(program->selectedNode - ((program->tableMode == FULL) ? (chunckSize) : (1)), 0, (int)program->fecNotes.size);
            break;
        case KEY_RIGHT:
            if (program->findMode)
                program->selectedNode = clamp(program->selectedNode + ((program->tableMode == FULL) ? (chunckSize) : (1)), 0, (int)program->entries.size - 1);
            else
                program->selectedNode = clamp(program->selectedNode + ((program->tableMode == FULL) ? (chunckSize) : (1)), 0, (int)program->fecNotes.size);
            break;
        case KEY_UP:
            if (program->tableMode == FULL)
            {
                if (program->findMode)
                    program->selectedNode = clamp(program->selectedNode - 1, 0, (int)program->entries.size - 1);
                else
                    program->selectedNode = clamp(program->selectedNode - 1, 0, (int)program->fecNotes.size);
            }
            else
            {
                program->field = clamp(program->field - 1, 0, 5);
            }
            break;
        case KEY_DOWN:
            if (program->tableMode == FULL)
            {
                if (program->findMode)
                    program->selectedNode = clamp(program->selectedNode + 1, 0, (int)program->entries.size - 1);
                else
                    program->selectedNode = clamp(program->selectedNode + 1, 0, (int)program->fecNotes.size);
            }
            else
            {
                program->field = clamp(program->field + 1, 0, 5);
            }
            break;
        case MY_KEY_END:
            if (program->findMode)
                program->selectedNode = program->entries.size - 1;
            else
                program->selectedNode = program->fecNotes.size;
            break;
        case MY_KEY_HOME:
            program->selectedNode = 0;
            break;
        case MY_KEY_UNDO:
            undo(&program->undoStack, &program->fecNotes);
            if (program->findMode)
                program->selectedNode = clamp(program->selectedNode, 0, (int)program->entries.size - 1);
            else
                program->selectedNode = clamp(program->selectedNode, 0, (int)program->fecNotes.size);
            break;
        case MY_KEY_REDO:
            redo(&program->undoStack, &program->fecNotes);
            if (program->findMode)
                program->selectedNode = clamp(program->selectedNode, 0, (int)program->entries.size - 1);
            else
                program->selectedNode = clamp(program->selectedNode, 0, (int)program->fecNotes.size);
            break;
        default:
            break;
        }
    }
    free_list(&program->fecNotes);
    free_array(&program->entries);
}

void save(ProgramInstance* program)
{
}

void sorting(ProgramInstance* program)
{
}

void find(ProgramInstance* program)
{
}

void save_result(ProgramInstance* program)
{
}

//#endif // _RELEASE


static void print_table(WINDOW* winTable, WINDOW* winRed, ProgramInstance* program)
{
    if (program->findMode)
        print_table_ref(winTable, &program->entries, program->selectedNode, program->tableMode);
    else
        print_table_list(winTable, &program->fecNotes, program->selectedNode, program->tableMode);
    if (program->tableMode == REDACTOR)
    {
        if (program->findMode)
            print_note_editor(winRed, *((FECNote**)get_ref(&program->entries, program->selectedNode)), program->field, program->selectedNode);
        else
            print_note_editor(winRed, get_at(&program->fecNotes, program->selectedNode), program->field, program->selectedNode);
    }
}

static void change_serialNumber(ProgramInstance* program)
{
    FECNote* note = get_at(&program->fecNotes, program->selectedNode);
    int initialValue = note->serialNumber;
    if (get_user_input_int(program->popUp, L"Серийный номер", &note->serialNumber))
    {
        note->serialNumber = initialValue;
        pop_up_notification(program->popUp, L"Ошибка при вводе целого числа.", N_ERR, POP_UP_Y);
        getch();
    }
}

static void change_factoryNumber(ProgramInstance* program)
{
    FECNote* note = get_at(&program->fecNotes, program->selectedNode);
    int initialValue = note->factoryNumber;
    if (get_user_input_int(program->popUp, L"Номер фабрики", &note->factoryNumber))
    {
        note->factoryNumber = initialValue;
        pop_up_notification(program->popUp, L"Ошибка при вводе целого числа.", N_ERR, POP_UP_Y);
        getch();
    }
}

static void change_directorFullName(ProgramInstance* program)
{
    char* str = get_user_input_str(program->popUp, L"ФИО директора (15 символов)");
    if (strlen(str) > 15)
    {
        pop_up_notification(program->popUp, L"Длина больше 15 символов. Строка будет обрезана.", N_INFO, POP_UP_Y);
        str[15] = '\0';
        getch();
    }
    strcpy(get_at(&program->fecNotes, program->selectedNode)->directorFullName, str);
    free(str);
}

static void change_engineerFulName(ProgramInstance* program)
{
    char* str = get_user_input_str(program->popUp, L"ФИО инженера (15 символов)");
    if (strlen(str) > 15)
    {
        pop_up_notification(program->popUp, L"Длина больше 15 символов. Строка будет обрезана.", N_INFO, POP_UP_Y);
        str[15] = '\0';
        getch();
    }
    strcpy(get_at(&program->fecNotes, program->selectedNode)->engineerFullName, str);
    free(str);
}

static void change_energyConsPlan(ProgramInstance* program)
{
    FECNote* note = get_at(&program->fecNotes, program->selectedNode);
    float initialValue = note->energyConsPlan;
    if (get_user_input_float(program->popUp, L"План расхода энергии", &note->energyConsPlan))
    {
        note->energyConsPlan = initialValue;
        pop_up_notification(program->popUp, L"Ошибка при вводе вещественного числа.", N_ERR, POP_UP_Y);
        getch();
    }
}

static void change_energyConsReal(ProgramInstance* program)
{
    FECNote* note = get_at(&program->fecNotes, program->selectedNode);
    float initialValue = note->energyConsReal;
    if (get_user_input_float(program->popUp, L"Фактисеский расход энергии", &note->energyConsReal))
    {
        note->energyConsReal = initialValue;
        pop_up_notification(program->popUp, L"Ошибка при вводе вещественного числа.", N_ERR, POP_UP_Y);
        getch();
    }
}
