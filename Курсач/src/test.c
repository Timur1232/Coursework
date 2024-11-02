#ifdef _TEST

#include <curses.h>

#include <stdio.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>

#include <macros.h>
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

    WINDOW *winTable = newwin(LINES, 117, 0, 19),
           *winMenu = newwin(LINES, 19, 0, 0),
           *popUp = newwin(5, 15, 0, 0);
    refresh();
    keypad(winTable, TRUE);
    keypad(winMenu, TRUE);


    List list = init_list();
    scan_note_list("input/random_gen.txt", &list);
    int field = 0;
    int noteIndex = 0;

    while (1)
    {
        print_note_editor(winTable, get_at(&list, noteIndex), field);
        int ch = getch();

        switch (ch)
        {
        case KEY_UP:
            field = clamp(field - 1, 0, 4);
            break;
        case KEY_DOWN:
            field = clamp(field + 1, 0, 4);
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

    getch();

    //RefArray entries = init_ref_array(0);

    //int chunck = 0;
    //int highlightElem = 0;
    //int printList = 1;
    //int x = 5, y = 10;
    //while (1)
    //{
    //    print_menu(winMenu, &menu);
    //    if (printList)
    //        print_table_list(winTable, &list, chunck, highlightElem);
    //    else
    //        print_table_ref(winTable, &entries, chunck, highlightElem);
    //    //pop_up_notification(popUp, L"Тестовое сообщение...", x, y);
    //    int ch = getch();

    //    switch (ch)
    //    {
    //    case KEY_LEFT:
    //        if (printList)
    //            chunck = clamp(chunck - 1, 0, list.size / CHUNCK_SIZE);
    //        else
    //            chunck = clamp(chunck - 1, 0, entries.size / CHUNCK_SIZE);
    //        break;
    //    case KEY_RIGHT:
    //        if (printList)
    //            chunck = clamp(chunck + 1, 0, list.size / CHUNCK_SIZE);
    //        else
    //            chunck = clamp(chunck + 1, 0, entries.size / CHUNCK_SIZE);
    //        break;
    //    case KEY_UP:
    //        highlightElem = clamp(highlightElem - 1, -1, CHUNCK_SIZE);
    //        if (highlightElem == -1)
    //        {
    //            highlightElem = chunck != 0 ? CHUNCK_SIZE - 1 : 0;
    //            if (printList)
    //                chunck = clamp(chunck - 1, 0, list.size / CHUNCK_SIZE);
    //            else
    //                chunck = clamp(chunck - 1, 0, entries.size / CHUNCK_SIZE);
    //        }
    //        break;
    //    case KEY_DOWN:
    //        highlightElem = clamp(highlightElem + 1, 0, CHUNCK_SIZE);
    //        if (highlightElem == CHUNCK_SIZE)
    //        {
    //            if (printList)
    //                highlightElem = (chunck == list.size / CHUNCK_SIZE) ? CHUNCK_SIZE - 1 : 0;
    //            else
    //                highlightElem = (chunck == entries.size / CHUNCK_SIZE) ? CHUNCK_SIZE - 1 : 0;
    //            if (printList)
    //                chunck = clamp(chunck + 1, 0, list.size / CHUNCK_SIZE);
    //            else
    //                chunck = clamp(chunck + 1, 0, entries.size / CHUNCK_SIZE);
    //        }
    //        break;
    //    case 'w':
    //        y--;
    //        break;
    //    case 's':
    //        y++;
    //        break;
    //    case 'a':
    //        x--;
    //        break;
    //    case 'd':
    //        x++;
    //        break;
    //    case '1':
    //        printList = printList ? 0 : 1;
    //        chunck = 0;
    //        highlightElem = 0;
    //        if (!printList)
    //        {
    //            FECNote req = { 0, 0, "Timur Bai", "", 0.0f, 0.0f };
    //            entries = find_entries(&list, &req, dir_name);
    //        }
    //        else
    //        {
    //            clear_array(&entries);
    //        }
    //        break;
    //    default:
    //        break;
    //    }
    //}
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