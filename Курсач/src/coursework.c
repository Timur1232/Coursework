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
#include "find/find_entries.h"

static void print_table(WINDOW* winTable, WINDOW* winRed, ProgramInstance* program);

static void proccess_movement(ProgramInstance* program, int ch);
static void proccess_redacting(ProgramInstance* program, int ch);
static int proccess_menu(ProgramInstance* program, Menu* menu, int ch);

static char* construct_file_path(const char* fileName, const char* folderPath, const char* fileExtention);

static void dance();

static void add_note(ProgramInstance* program, FECNote* note);
static void delete_note(ProgramInstance* program);

static int (* const CHANGE_FUNC_ARRAY[6]) (ProgramInstance* program, FECNote* note) = {
    change_serialNumber, change_factoryNumber, change_directorFullName,
    change_engineerFulName, change_energyConsPlan, change_energyConsReal
};

static MenuCommand mainCom[] = {
    {L" ����� ������     ", new_list},
    {L" ��������� ������ ", load_list},
    //{L"���������", settings}
};

static Menu mainMenu = {
    0,
    (SCREEN_HEIGHT / 2 - 5), -7, -1, 0,
    L" ��������� ",
    L" �����     ",
    mainCom,
    sizeof(mainCom) / sizeof(MenuCommand),
    MIDDLE
};

static MenuCommand browsingCom[] = {
    {L" ���������  ", save},
    {L" ���������� ", sorting},
    {L" �����      ", find},
};

static Menu browsingMenu = {
    0,
    3, 0, -1, 0,
    L" �������� ",
    L" �����    ",
    browsingCom,
    sizeof(browsingCom) / sizeof(MenuCommand),
    LEFT
};

static void blanc(ProgramInstance* program)
{
}

static MenuCommand fieldCom[] = {
    {L" �������� ����� ", blanc},
    {L" ����� �������  ", blanc},
    {L" ��� ���������  ", blanc},
    {L" ��� ��������   ", blanc},
    {L" ����           ", blanc},
    {L" ������         ", blanc},
};

static Menu fieldChooseMenu = {
    0,
    3, 0, -1, 1,
    L" �������� ����:",
    L" ������          ",
    fieldCom,
    sizeof(fieldCom) / sizeof(MenuCommand),
    LEFT
};


ProgramInstance init_program()
{
    ProgramInstance program;

    // ������ � ������ ���������
    program.fecNotes = init_list();
    program.entries = init_ref_array(0);
    program.currentFileName = "";
    program.undoStack = init_undo();
    program.copyNote = init_note();
    program.copied = false;

    // ������ �� �������
    program.selectedNode = 0;
    program.field = 0;
    program.findMode = false;
    program.chunckSize = CHUNCK_SIZE_FULL;
    program.focus = FOCUS_BROWSING;
    program.shouldClose = false;
    program.saved = true;
    program.sortMode = 1;

    program.popUp = newwin(5, 15, 0, 0);
    program.winMain = stdscr;
    program.winTable = newwin(TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH, TABLE_WIN_Y, TABLE_WIN_X),
    program.winRed = newwin(REDACTOR_MENU_WIN_HEIGHT, REDACTOR_MENU_WIN_WIDTH, REDACTOR_MENU_WIN_Y, REDACTOR_MENU_WIN_X),
    program.winMenu = newwin(BROWSING_MENU_WIN_HEIGHT, BROWSING_MENU_WIN_WIDTH, 0, 0);
    program.winControls = newwin(CONTROLS_HEIGHT, BROWSING_MENU_WIN_WIDTH, BROWSING_MENU_WIN_HEIGHT, 0);
    program.winField = newwin(12, 21, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 8, SCREEN_WIDTH / 2 - 10);
    refresh();

    return program;
}

int Main(int argc, char** argv)
{
    setlocale(LC_CTYPE, "");
    system("mkdir files && mkdir files\\results");

    // ������������� PDCurses
    initscr();
    noecho();
    curs_set(0);
    mouse_set(ALL_MOUSE_EVENTS);
    keypad(stdscr, TRUE);
    timeout(200);
    resize_term(SCREEN_HEIGHT, SCREEN_WIDTH);
    if (has_colors())
    {
        start_color();
        init_pair(0, COLOR_WHITE, COLOR_BLACK);
        init_pair(N_INFO, COLOR_BLUE, COLOR_WHITE);
        init_pair(N_ERR, COLOR_WHITE, COLOR_RED);
    }

    ProgramInstance program = init_program();

    while (!program.shouldClose)
    {
        print_menu(program.winMain, &mainMenu);
        dance();
        int ch = getch();
        proccess_menu(&program, &mainMenu, ch);
        timeout(200);
    }

    endwin();
    return 0;
}

void new_list(ProgramInstance* program)
{
    timeout(-1);
    program->currentFileName = get_user_input_str(program->popUp, L"������� �������� �����:", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
    if (strlen(program->currentFileName) == 0)
    {
        DELETE(program->currentFileName);
        program->currentFileName = "";
        pop_up_notification_wchar(program->popUp, L"������ ������.", N_ERR, POP_UP_Y);
        getch();
        return;
    }
    char* filePath = construct_file_path(program->currentFileName, "files/", ".txt");
    FILE* f = fopen(filePath, "rt");
    DELETE(filePath);
    if (f)
    {
        fclose(f);
        pop_up_notification_wchar(program->popUp, L"���� ��� ����������.", N_INFO, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
        char* ans = get_user_input_str(program->popUp, L"��������? (Y/N)", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 + 4);
        if (ans[0] == 'Y' || ans[0] == 'y')
        {
            DELETE(ans);
        }
        else if (ans[0] == 'N' || ans[0] == 'n')
        {
            DELETE(ans);
            return;
        }
        else
        {
            DELETE(ans);
            pop_up_notification_wchar(program->popUp, L"������������ ����.", N_ERR, POP_UP_Y);
            getch();
            return;
        }
    }
    list_redactor(program);
    DELETE(program->currentFileName);
    program->currentFileName = "";
}

void load_list(ProgramInstance* program)
{
    timeout(-1);
    program->currentFileName = get_user_input_str(program->popUp, L"������� ���� �� ����� � ����� files/ (� �����������):", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
    char* findExtention = strrchr(program->currentFileName, '.');
    if (!findExtention)
    {
        print_menu(program->winMain, &mainMenu);
        pop_up_notification_wchar(program->popUp, L"�� ������� ����������.", N_ERR, POP_UP_Y);
        DELETE(program->currentFileName);
        program->currentFileName = "";
        getch();
        return;
    }
    char* fileExtention[10] = { 0 };
    strcpy(fileExtention, findExtention);
    *findExtention = '\0';
    char* filePath = construct_file_path(program->currentFileName, "files/", fileExtention);
    if (!filePath)
    {
        LOG_DEBUG(LOG_ERR, "coursework.c", "load_list()", "malloc() returned NULL", LOG_FILE);
        pop_up_notification_wchar(program->popUp, L"������� malloc() ������� NULL.", N_ERR, POP_UP_Y);
        getch();
        return;
    }
    else if (strcmp(fileExtention, ".txt") == 0)
    {
        ErrorHandler err = scan_note_list(filePath, &program->fecNotes);
        if (err.err != ALL_GOOD)
        {
            char buff[100] = { 0 };
            print_menu(program->winMain, &mainMenu);
            pop_up_notification_wchar(program->popUp, L"������ ������ �����.", N_ERR, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
            pop_up_notification(program->popUp, proccess_error(buff, err), N_ERR, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 + 3);
            DELETE(filePath);
            getch();
            return;
        }
    }
    else if (strcmp(fileExtention, ".fec") == 0)
    {
        if (scan_bin_note_list(filePath, &program->fecNotes))
        {
            print_menu(program->winMain, &mainMenu);
            pop_up_notification_wchar(program->popUp, L"������ ������ �����.", N_ERR, POP_UP_Y);
            DELETE(program->currentFileName);
            DELETE(filePath);
            program->currentFileName = "";
            getch();
            return;
        }
    }
    DELETE(filePath);
    list_redactor(program);
    DELETE(program->currentFileName);
    program->currentFileName = "";
}

void list_redactor(ProgramInstance* program)
{
    clear();
    keypad(program->winTable, TRUE);
    keypad(program->winMenu, TRUE);
    refresh();    

    sort(&program->fecNotes, COMPARE_FUNC_ARRAY_ASC[0]);
    browsingMenu.selected = -1;
    while (1)
    {
        if (!program->saved)
        {
            browsingMenu.commands[0].text = L" *��������� ";
        }
        else
        {
            browsingMenu.commands[0].text = L" ���������  ";
        }

        print_menu(program->winMenu, &browsingMenu);
        print_controls(program->winControls, program->focus);
        print_table(program->winTable, program->winRed, program);

        int ch = getch();
        if (ch == '\t')
        {
            if (program->focus == FOCUS_MENU)
            {
                program->focus = FOCUS_BROWSING;
                browsingMenu.selected = -1;
            }
            else
            {
                program->focus = FOCUS_MENU;
                browsingMenu.selected = 0;
            }
            program->findMode = false;
        }
        if (program->focus != FOCUS_MENU)
        {
            proccess_movement(program, ch);
            proccess_redacting(program, ch);
        }
        else
        {
            proccess_menu(program, &browsingMenu, ch);
            if (program->shouldClose)
            {
                if (program->saved)
                {
                    break;
                }
                else
                {
                    char* str = get_user_input_str(program->popUp, L"��������� ���������? (Y/N):", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
                    if (str[0] == 'Y' || str[0] == 'y')
                    {
                        save(program);
                        DELETE(str);
                        break;
                    }
                    else if (str[0] == 'N' || str[0] == 'n')
                    {
                        DELETE(str);
                        break;
                    }
                    else
                    {
                        DELETE(str);
                        pop_up_notification_wchar(program->popUp, L"������������ ����.", N_ERR, POP_UP_Y);
                        getch();
                    }
                }
            }
        }
    }
    program->selectedNode = 0;
    program->field = 0;
    program->findMode = false;
    program->chunckSize = CHUNCK_SIZE_FULL;
    program->focus = FOCUS_BROWSING;
    program->shouldClose = false;

    browsingMenu.selected = 0;

    free_list(&program->fecNotes);
    free_array(&program->entries);
}

void save(ProgramInstance* program)
{
    if (!program->saved)
    {
        char* filePathText = construct_file_path(program->currentFileName, "files/", ".txt");
        char* filePathBin = construct_file_path(program->currentFileName, "files/", ".fec");
        save_note_list(filePathText, &program->fecNotes);
        save_bin_note_list(filePathBin, &program->fecNotes);
        DELETE(filePathText);
        DELETE(filePathBin);
    }
    if (program->fecNotes.size)
    {
        char* resultsName = NEW(char, strlen(program->currentFileName) + 6);
        if (!resultsName)
        {
            LOG_DEBUG(LOG_ERR, "coursework.c", "save()", "malloc() returned NULL", LOG_FILE);
            return;
        }
        strcpy(resultsName, program->currentFileName);
        strcat(resultsName, "_res");
        char* resultsPath = construct_file_path(resultsName, "files/results/", ".txt");
        if (!resultsPath)
        {
            DELETE(resultsName);
            LOG_DEBUG(LOG_ERR, "coursework.c", "save()", "malloc() returned NULL", LOG_FILE);
            pop_up_notification_wchar(program->popUp, L"������� malloc() ������� NULL.", N_ERR, POP_UP_Y);
            getch();
            return;
        }
        save_results(resultsPath, &program->fecNotes);
        DELETE(resultsName);
        DELETE(resultsPath);
    }
    program->saved = true;
}

void sorting(ProgramInstance* program)
{
    while (true)
    {
        print_menu(program->winField, &fieldChooseMenu);
        mvwaddch(program->winField, 3 + abs(program->sortMode) - 1, 1, (program->sortMode < 0) ? '<' : '>');
        wrefresh(program->winField);
        int ch = getch();
        int res = proccess_menu(program, &fieldChooseMenu, ch);
        if (program->shouldClose)
        {
            program->shouldClose = false;
            return;
        }
        if (res >= 0)
        {
            if (res == (abs(program->sortMode) - 1) && program->sortMode > 0)
            {
                sort(&program->fecNotes, COMPARE_FUNC_ARRAY_DESC[res]);
                program->sortMode *= -1;
                return;
            }
            program->sortMode = res + 1;
            sort(&program->fecNotes, COMPARE_FUNC_ARRAY_ASC[res]);
            return;
        }
    }
}

void find(ProgramInstance* program)
{
    if (program->findMode)
    {
        clear_array(&program->entries);
        program->findMode = false;
        program->focus = FOCUS_BROWSING;
        return;
    }
    while (true)
    {
        print_menu(program->winField, &fieldChooseMenu);
        int ch = getch();
        int res = proccess_menu(program, &fieldChooseMenu, ch);
        if (program->shouldClose)
        {
            program->shouldClose = false;
            return;
        }
        if (res >= 0)
        {
            FECNote note = init_note();
            int err = CHANGE_FUNC_ARRAY[res](program, &note);
            if (err)
            {
                return;
            }
            find_entries(&program->fecNotes, &program->entries, &note, COMPARE_FUNC_ARRAY_ASC[res]);
            if (program->entries.size == 0)
            {
                pop_up_notification_wchar(program->popUp, L"��������� �� ������.", N_INFO, POP_UP_Y);
                getch();
            }
            else
            {
                program->findMode = true;
                program->focus = FOCUS_FIND;
            }
            return;
        }
    }
}

//__________________________________[����������� �������]__________________________________//

void proccess_movement(ProgramInstance* program, int ch)
{
    switch (ch)
    {
    case KEY_LEFT:
        if (program->findMode)
            program->selectedNode = clamp(program->selectedNode - ((program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND) ? (program->chunckSize) : (1)), 0, (int)program->entries.size - 1);
        else
            program->selectedNode = clamp(program->selectedNode - ((program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND) ? (program->chunckSize) : (1)), 0, (int)program->fecNotes.size);
        break;
    case KEY_RIGHT:
        if (program->findMode)
            program->selectedNode = clamp(program->selectedNode + ((program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND) ? (program->chunckSize) : (1)), 0, (int)program->entries.size - 1);
        else
            program->selectedNode = clamp(program->selectedNode + ((program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND) ? (program->chunckSize) : (1)), 0, (int)program->fecNotes.size);
        break;
    case KEY_UP:
        if (program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND)
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
        if (program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND)
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
    }
}

void proccess_redacting(ProgramInstance* program, int ch)
{
    switch (ch)
    {
    case MY_KEY_ESCAPE:
        if (program->focus == FOCUS_EDITOR)
        {
            program->focus = FOCUS_BROWSING;
            program->chunckSize = CHUNCK_SIZE_FULL;
        }
        break;
    case '\n':
        if (program->focus == FOCUS_EDITOR && program->selectedNode != program->fecNotes.size)
        {
            FECNote* note;
            if (program->findMode)
            {
                note = *((FECNote**)get_ref(&program->entries, program->selectedNode));
            }
            else
            {
                note = get_at(&program->fecNotes, program->selectedNode);
            }
            push_action(&program->undoStack, UNDO_CHANGE, note, program->selectedNode);
            CHANGE_FUNC_ARRAY[program->field](program, note);
            program->saved = false;
        }
        else if (program->focus == FOCUS_BROWSING || program->focus == FOCUS_FIND)
        {
            program->focus = FOCUS_EDITOR;
            program->chunckSize = CHUNCK_SIZE_REDACTOR;
        }
        if (program->selectedNode == program->fecNotes.size)
        {
            FECNote note = init_note();
            add_note(program, &note);
            program->focus = FOCUS_EDITOR;
            program->saved = false;
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
            add_note(program, &program->copyNote);
            program->saved = false;
        }
        break;
    case MY_KEY_CTRL_D:
    case MY_KEY_DEL:
        if (program->fecNotes.size)
        {
            delete_note(program);
            program->saved = false;
        }
        break;
    case MY_KEY_CTRL_A:
    case MY_KEY_INS:
    {
        FECNote note = init_note();
        add_note(program, &note);
        program->focus = FOCUS_EDITOR;
        program->saved = false;
        break;
    }
    case MY_KEY_UNDO:
        if (undo(&program->undoStack, &program->fecNotes) != UNDO_EMPTY)
            program->saved = false;
        if (program->findMode)
            program->selectedNode = clamp(program->selectedNode, 0, (int)program->entries.size - 1);
        else
            program->selectedNode = clamp(program->selectedNode, 0, (int)program->fecNotes.size);
        break;
    case MY_KEY_REDO:
        if (redo(&program->undoStack, &program->fecNotes) != UNDO_EMPTY)
            program->saved = false;
        if (program->findMode)
            program->selectedNode = clamp(program->selectedNode, 0, (int)program->entries.size - 1);
        else
            program->selectedNode = clamp(program->selectedNode, 0, (int)program->fecNotes.size);
        break;
    case MY_KEY_SAVE:
        save(program);
        break;
    case MY_KEY_CTRL_F:
        find(program);
        break;
    case MY_KEY_CTRL_G:
        sorting(program);
        break;
    default:
        break;
    }
}

int proccess_menu(ProgramInstance* program, Menu* menu, int ch)
{
    switch (ch)
    {
    case KEY_UP:
        menu->selected = loop(menu->selected - 1, 0, menu->commandsSize);
        break;
    case KEY_DOWN:
        menu->selected = loop(menu->selected + 1, 0, menu->commandsSize);
        break;
    case '\n':
        if (menu->selected == menu->commandsSize)
        {
            program->shouldClose = true;
            break;
        }
        menu->commands[menu->selected].function(program);
        return menu->selected;
        break;
    case MY_KEY_ESCAPE:
        program->shouldClose = true;
        break;
    default:
        break;
    }
    return -1;
}

char* construct_file_path(const char* fileName, const char* folderPath, const char* fileExtention)
{
    char* filePath = (char*)malloc(strlen(fileName) + strlen(folderPath) + strlen(fileExtention) + 1);
    if (!filePath)
    {
        return NULL;
    }
    strcpy(filePath, folderPath);
    strcat(filePath, fileName);
    strcat(filePath, fileExtention);
    return filePath;
}

static const wchar_t* const dance_frames[] = {
    L"<(�_� )/ ",
    L" /(�_�)/ ",
    L" \\( �_�)>",
    L" \\(�_�)\\ "
};

void dance()
{
    static int frame = 0;
    for (int i = 3; i < SCREEN_WIDTH - 9; i += 10)
        mvaddwstr(1, i, dance_frames[frame]);
    frame = loop(frame + 1, 0, 3);
}

static void print_table(WINDOW* winTable, WINDOW* winRed, ProgramInstance* program)
{
    if (program->findMode)
        print_table_ref(winTable, &program->entries, program->selectedNode, program->focus);
    else
        print_table_list(winTable, &program->fecNotes, program->selectedNode, program->focus);
    if (program->focus == FOCUS_EDITOR)
    {
        if (program->findMode)
            print_note_editor(winRed, *((FECNote**)get_ref(&program->entries, program->selectedNode)), program->field, program->selectedNode);
        else
            print_note_editor(winRed, get_at(&program->fecNotes, program->selectedNode), program->field, program->selectedNode);
    }
}

int change_serialNumber(ProgramInstance* program, FECNote* note)
{
    int initialValue = note->serialNumber;
    if (get_user_input_int(program->popUp, L"�������� �����", &note->serialNumber))
    {
        note->serialNumber = initialValue;
        pop_up_notification_wchar(program->popUp, L"������ ��� ����� ������ �����.", N_ERR, POP_UP_Y);
        getch();
        return -1;
    }
    return 0;
}

int change_factoryNumber(ProgramInstance* program, FECNote* note)
{
    int initialValue = note->factoryNumber;
    if (get_user_input_int(program->popUp, L"����� �������", &note->factoryNumber))
    {
        note->factoryNumber = initialValue;
        pop_up_notification_wchar(program->popUp, L"������ ��� ����� ������ �����.", N_ERR, POP_UP_Y);
        getch();
        return -1;
    }
    return 0;
}

int change_directorFullName(ProgramInstance* program, FECNote* note)
{
    char* str = get_user_input_str(program->popUp, L"��� ��������� (15 ��������)", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
    if (strlen(str) > 15)
    {
        pop_up_notification_wchar(program->popUp, L"����� ������ 15 ��������. ������ ����� ��������.", N_INFO, POP_UP_Y);
        str[15] = '\0';
        getch();
        return -1;
    }
    strcpy(note->directorFullName, str);
    free(str);
    return 0;
}

int change_engineerFulName(ProgramInstance* program, FECNote* note)
{
    char* str = get_user_input_str(program->popUp, L"��� �������� (15 ��������)", SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4);
    if (strlen(str) > 15)
    {
        pop_up_notification_wchar(program->popUp, L"����� ������ 15 ��������. ������ ����� ��������.", N_INFO, POP_UP_Y);
        str[15] = '\0';
        getch();
        return -1;
    }
    strcpy(note->engineerFullName, str);
    free(str);
    return 0;
}

int change_energyConsPlan(ProgramInstance* program, FECNote* note)
{
    float initialValue = note->energyConsPlan;
    if (get_user_input_float(program->popUp, L"���� ������� �������", &note->energyConsPlan))
    {
        note->energyConsPlan = initialValue;
        pop_up_notification_wchar(program->popUp, L"������ ��� ����� ������������� �����.", N_ERR, POP_UP_Y);
        getch();
        return -1;
    }
    return 0;
}

int change_energyConsReal(ProgramInstance* program, FECNote* note)
{
    float initialValue = note->energyConsReal;
    if (get_user_input_float(program->popUp, L"����������� ������ �������", &note->energyConsReal))
    {
        note->energyConsReal = initialValue;
        pop_up_notification_wchar(program->popUp, L"������ ��� ����� ������������� �����.", N_ERR, POP_UP_Y);
        getch();
        return -1;
    }
    return 0;
}

void add_note(ProgramInstance* program, FECNote* note)
{
    push_action(&program->undoStack, UNDO_ADD, note, program->selectedNode);
    insert(&program->fecNotes, note, program->selectedNode);
}

void delete_note(ProgramInstance* program)
{
    push_action(&program->undoStack, UNDO_DEL, get_at(&program->fecNotes, program->selectedNode), program->selectedNode);
    pop(&program->fecNotes, program->selectedNode);
}
