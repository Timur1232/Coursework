#ifdef _TEST

#include <curses.h>

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include <macros.h>
#include <types.h>
#include "interface/interface.h"
#include "list/list.h"
#include "file_work/file_work.h"
//#include "find/find_entries.h"
//#include "ref_array/ref_array.h"
//#include "compare/compare.h"
//#include "log/log.h"
#include "user_input/user_input.h"
#include "undo_stack/undo_stack.h"

#if 0

void f()
{
    printw("Func");
}

MenuCommand commands[] = {
    {L"Новый список", TRUE/*, f*/},
    {L"Загрузить список", FALSE/*, f*/}
};

Menu menu = {
    3, 0, 1, 1,
    L"Программа",
    L"Выход",
    commands,
    sizeof(commands) / sizeof(MenuCommand),
    LEFT
};

int main()
{
    setlocale(LC_CTYPE, "");

    initscr();
    noecho();
    curs_set(0);
    mouse_set(ALL_MOUSE_EVENTS);
    keypad(stdscr, TRUE);
    //timeout(32);
    resize_term(40, 136);
    start_color();

    clear();

    WINDOW *winTable = newwin(TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH, TABLE_WIN_Y, TABLE_WIN_X),
           *winRed = newwin(15, 117, 0, 19),
           *winMenu = newwin(LINES, 19, 0, 0),
           *popUp = newwin(5, 15, 0, 0);
    refresh();
    keypad(winTable, TRUE);
    keypad(winMenu, TRUE);


    List list = init_list();
    ErrorHandler err = scan_note_list("input/random_gen.txt", &list);
    char buff[100] = { 0 };
    if (err.err != ALL_GOOD)
    {
        LOG_DEBUG(ERR, "test.c", "main()", proccess_error(buff, err), LOG_FILE);
        endwin();
        return -1;
    }
    int field = 0;
    int noteIndex = 0;

    /*while (1)
    {
        print_note_editor(winTable, get_at(&list, noteIndex), field, noteIndex);
        int ch = getch();

        switch (ch)
        {
        case KEY_UP:
            field = clamp(field - 1, 0, 5);
            break;
        case KEY_DOWN:
            field = clamp(field + 1, 0, 5);
            break;
        case KEY_LEFT:
            noteIndex = clamp(noteIndex - 1, 0, (int)(list.size - 1));
            break;
        case KEY_RIGHT:
            noteIndex = clamp(noteIndex + 1, 0, (int)(list.size - 1));
            break;
        default:
            break;
        }
    }

    getch();*/

    RefArray entries = init_ref_array(50);

    int selected = 0;
    int chunckSize = CHUNCK_SIZE_FULL;
    TableMode mode = FULL;
    int printList = 1;
    int x = 5, y = 10;
    while (1)
    {
        print_menu(winMenu, &menu);

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
        //pop_up_notification(popUp, L"Тестовое сообщение...", x, y);
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
    //wrefresh(win);
    //refresh();

    free_list(&list);
    free_array(&entries);
    //int highlight = 0;
    /*while (1)
    {
        print_menu(stdscr, &menu);
        int ch = getch();

        if (ch == KEY_UP)
        {
            highlight--;
            if (highlight < 0)
                highlight = 0;
        }
        else if (ch == KEY_DOWN)
        {
            highlight++;
            if (highlight >= menu.commandsSize)
                highlight = menu.commandsSize - 1;
        }
        else if (ch == '\n')
        {
            menu.commands[highlight].function();
            break;
        }
        highlight_on_index(&menu, highlight);
    }*/

    getch();

    endwin();
    return 0;
}
#else
int main()
{
    setlocale(LC_CTYPE, "");

    initscr();
    noecho();
    curs_set(0);
    mouse_set(ALL_MOUSE_EVENTS);
    keypad(stdscr, TRUE);
    //timeout(32);
    resize_term(40, 136);
    start_color();

    /*while (1)
    {
        printw("%d\n", getch());
    }*/

    WINDOW* winTable = newwin(TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH, TABLE_WIN_Y, TABLE_WIN_X),
        * popUp = newwin(5, 15, 0, 0);

    UndoStack stack = init_undo();
    List list = init_list();

    scan_note_list("input/random_gen.txt", &list);
    char* str;
    int index = 0;

    while (1)
    {
        print_table_list(winTable, &list, 0, FULL);
        int ch = getch();
        switch (ch)
        {
        case 'd':
            str = get_user_input(popUp, L"Номер");
            sscanf(str, "%d", &index);
            free(str);
            index--;
            push_action(&stack, UNDO_DEL, get_at(&list, index), index);
            pop(&list, index);
            break;
        case 'a':
            str = get_user_input(popUp, L"Номер");
            sscanf(str, "%d", &index);
            free(str);
            index--;
            FECNote note = init_note();
            push_action(&stack, UNDO_ADD, &note, index);
            insert(&list, &note, index);
            break;
        case MY_KEY_UNDO:
            undo(&stack, &list);
            break;
        case MY_KEY_REDO:
            redo(&stack, &list);
            break;
        default:
            break;
        }
    }
    endwin();
    return 0;
}
#endif

#endif