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

static MenuCommand mainCom[] = {
    {L" Новый список     ", TRUE, new_list},
    {L" Загрузить список ", FALSE, load_list},
    //{L"Настройки", FALSE, settings}
};

static const Menu mainMenu = {
    (SCREEN_HEIGHT / 2 - 5), -7, -1, 1,
    L" Программа ",
    L" Выход     ",
    mainCom,
    sizeof(mainCom) / sizeof(MenuCommand),
    MIDDLE
};

static MenuCommand browsingCom[] = {
    {L" Сохранить  ", TRUE, save},
    {L" Сортировка ", FALSE, sorting},
    {L" Найти      ", FALSE, find},
};

static const Menu browsingMenu = {
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
        start_color();

    ProgramInstance program = init_program();

    WINDOW* menuWin = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);

    int selected = 0;
    Bool shouldClose = false;
    while (!shouldClose)
    {
        highlight_on_index(&mainMenu, selected);
        print_menu(menuWin, &mainMenu);
        int ch = getch();

        switch (ch)
        {
        case KEY_UP:
            selected = loop(selected - 1, 0, mainMenu.commandsSize);
            break;
        case KEY_DOWN:
            selected = loop(selected + 1, 0, mainMenu.commandsSize);
            break;
        case '\n':
            if (selected == mainMenu.commandsSize)
            {
                shouldClose = true;
                break;
            }
            mainMenu.commands[selected].function();
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
    list_redactor(NULL);
}

void load_list(ProgramInstance* program)
{
    WINDOW* popUp = newwin(5, 15, 0, 0);
    char* str = get_user_input(popUp, L"Введите название файла.");

    list_redactor(str);

    free(str);
    delwin(popUp);
}

void list_redactor(ProgramInstance* program)
{
    clear();

    WINDOW* winTable = newwin(TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH, TABLE_WIN_Y, TABLE_WIN_X),
        * winRed = newwin(15, 117, 0, 19),
        * winMenu = newwin(LINES, 19, 0, 0),
        * popUp = newwin(5, 15, 0, 0);
    refresh();
    keypad(winTable, TRUE);
    keypad(winMenu, TRUE);


    List list = init_list();
    char* filePath = (char*)_alloca(strlen(fileName) + strlen("input/.txt") + 1);
    strcpy(filePath, "input/");
    strcat(filePath, fileName);
    strcat(filePath, ".txt");
    ErrorHandler err = scan_note_list(filePath, &list);
    if (err.err != ALL_GOOD)
    {
        char buff[100] = { 0 };
        LOG_DEBUG(ERR, "test.c", "main()", proccess_error(buff, err), LOG_FILE);
        endwin();
        return;
    }
    int field = 0;
    int noteIndex = 0;

    RefArray entries = init_ref_array(50);

    int selected = 0;
    int chunckSize = CHUNCK_SIZE_FULL;
    TableMode mode = FULL;
    int printList = 1;
    int x = 5, y = 10;
    while (1)
    {
        print_menu(winMenu, &browsingMenu);

        if (printList)
            print_table_list(winTable, &list, selected, mode);
        else
            print_table_ref(winTable, &entries, selected, mode);
        if (mode == REDACTOR)
        {
            if (printList)
                print_note_editor(winRed, get_at(&list, selected), field, selected);
            else
                print_note_editor(winRed, *((FECNote**)get_ref(&entries, selected)), field, selected);
        }
        int ch = getch();

        switch (ch)
        {
        case '\n':
            if (mode == REDACTOR)
            {
                mode = FULL;
                chunckSize = CHUNCK_SIZE_FULL;
            }
            else
            {
                mode = REDACTOR;
                chunckSize = CHUNCK_SIZE_REDACTOR;
            }
            break;
        case KEY_LEFT:
            if (printList)
                selected = clamp(selected - ((mode == FULL) ? (chunckSize) : (1)), 0, (int)list.size);
            else
                selected = clamp(selected - ((mode == FULL) ? (chunckSize) : (1)), 0, (int)entries.size - 1);
            break;
        case KEY_RIGHT:
            if (printList)
                selected = clamp(selected + ((mode == FULL) ? (chunckSize) : (1)), 0, (int)list.size);
            else
                selected = clamp(selected + ((mode == FULL) ? (chunckSize) : (1)), 0, (int)entries.size - 1);
            break;
        case KEY_UP:
            if (mode == FULL)
            {
                if (printList)
                    selected = clamp(selected - 1, 0, (int)list.size);
                else
                    selected = clamp(selected - 1, 0, (int)entries.size - 1);
            }
            else
            {
                field = clamp(field - 1, 0, 5);
            }
            break;
        case KEY_DOWN:
            if (mode == FULL)
            {
                if (printList)
                    selected = clamp(selected + 1, 0, (int)list.size);
                else
                    selected = clamp(selected + 1, 0, (int)entries.size - 1);
            }
            else
            {
                field = clamp(field + 1, 0, 5);
            }
            break;
        case '1':
            printList = printList ? 0 : 1;
            selected = 0;
            if (!printList)
            {
                FECNote req = { 0, 0, "Timur Bai", "", 0.0f, 0.0f };
                find_entries(&list, &entries, &req, dir_name);
            }
            else
            {
                clear_array(&entries);
            }
            break;
        default:
            break;
        }
    }
    free_list(&list);
    free_array(&entries);
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