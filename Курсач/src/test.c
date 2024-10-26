#ifdef _TEST

#include <curses.h>

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "interface/interface.h"
#include "list/list.h"
#include "file_work/file_work.h"
#include "find/find_entries.h"
#include "ref_array/ref_array.h"
#include "compare/compare.h"

#if 1

void f()
{
    printw("Func");
}

MenuCommand commands[] = {
    {L"Новый список", TRUE, f},
    {L"Загрузить список", FALSE, f},
    {L"Выход", FALSE, f}
};

Menu menu = {
    3, 0,
    L"Программа",
    commands,
    3,
    LEFT
};

static int clamp(int num, int min, int max)
{
    if (num < min)
        num = min;
    else if (num > max)
        num = max;
    return num;
}

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

    clear();

    WINDOW *winTable = newwin(LINES, 117, 0, 19),
           *winMenu = newwin(LINES, 19, 0, 0),
           *popUp = newwin(5, 15, 0, 0);
    refresh();
    keypad(winTable, TRUE);
    keypad(winMenu, TRUE);

    print_menu(winMenu, &menu);

    List list = init_list();
    scan_note_list("input/random_gen.txt", &list);
    RefArray entries = init_ref_array(0);

    int chunck = 0;
    int chunckSize = 16;
    int printList = 1;
    while (1)
    {
        raw();
        if (printList)
            print_table_list(winTable, &list, chunck);
        else
            print_table_ref(winTable, &entries, chunck);
        int ch = getch();

        switch (ch)
        {
        case KEY_LEFT:
            if (printList)
                chunck = clamp(chunck - 1, 0, list.size / chunckSize);
            else
                chunck = clamp(chunck - 1, 0, entries.size / chunckSize);
            break;
        case KEY_RIGHT:
            if (printList)
                chunck = clamp(chunck + 1, 0, list.size / chunckSize);
            else
                chunck = clamp(chunck + 1, 0, entries.size / chunckSize);
            break;
        case '1':
            printList = printList ? 0 : 1;
            chunck = 0;
            if (!printList)
            {
                FECNote req = { 0, 0, "Timur Bai", "", 0.0f, 0.0f };
                entries = find_entries(&list, &req, dir_name);
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
    //pop_up_notification(L"Тестовое сообщение", 5, 10);
    //wrefresh(win);
    //refresh();

    clear_list(&list);
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



    return 0;
}
#endif

#endif