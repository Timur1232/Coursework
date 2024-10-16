#ifdef _TEST

#include <curses.h>

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include "interface/interface.h"
#include "list/list.h"
#include "file_work/file_work.h"

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
    12, 0,
    L"Программа",
    commands,
    3,
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

    clear();

    WINDOW* win = newwin(LINES, 117, 0, 19);
    keypad(win, TRUE);
    refresh();
    //box(win, 0, 0);
    //wrefresh(win);

    List list = init_list();
    scan_note_list("intput/random_gen.txt", &list);

    print_table_list(win, &list, 20, 0);


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