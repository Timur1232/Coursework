#include "interface.h"

#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "../proccess_fec/proccess_fec.h"

static const char* const TOP_DIVIDER =      "┌───────┬──────────────┬──────────────┬─────────────────┬─────────────────┬─────────────┬─────────────┬─────────────┐";
static const char* const MIDDLE_DIVIDER =   "├───────┼──────────────┼──────────────┼─────────────────┼─────────────────┼─────────────┼─────────────┼─────────────┤";
static const char* const BOTTOM_DIVIDER =   "└───────┴──────────────┴──────────────┴─────────────────┴─────────────────┴─────────────┴─────────────┴─────────────┘";

void print_menu(WINDOW* win, Menu* menu)
{
    wclear(win);
    box(win, 0, 0);

    int startX = (menu->align == MIDDLE) ? (getmaxx(win) / 2) : 2;

    if (menu->title)
        mvwaddwstr(win,
            menu->commandStartY - 2,
            startX - ((menu->align == MIDDLE) ? (wcslen(menu->title) / 2) : 0),
            menu->title);

    for (int i = 0; i < menu->commandsSize; i++)
    {
        if (menu->commands[i].highlight)
            wattron(win, WA_REVERSE);

        mvwaddwstr(win,
            menu->commandStartY + i,
            startX + menu->commandShiftX,
            menu->commands[i].text);

        if (menu->commands[i].highlight)
            wattroff(win, WA_REVERSE);
    }

    wrefresh(win);
}

void print_table_list(WINDOW* win, ListPtr list, int num, int chunck)
{
    wclear(win);

    mvwaddstr(win, 0, 0,   TOP_DIVIDER);
    mvwaddwstr(win, 1, 0, L"│   #   │  Сер. номер  │ Номер завода │  ФИО директора  │  ФИО инженера   │    План     │   Расход    │ Отклоненине │");

    Iterator iter = get_iter(list, chunck * num);

    for (int i = 0; i < num; i++, INCREMENT(iter))
    {
        mvwprintw(win, i * 2 + 4, 0, MIDDLE_DIVIDER);
        mvwprintw(win, i * 2 + 3, 0, "│ %-5d │ %-12d │ %-12d │ %-15s │ %-15s │ %-11.2f │ %11.2f │ %11.2f │",
            i + 1, iter->data.serialNumber, iter->data.factoryNumber,
            iter->data.directorFullName, iter->data.engineerFullName,
            iter->data.energyConsPlan, iter->data.energyConsReal,
            calc_diff_deviation(&iter->data)
        );
    }
    mvwprintw(win, num * 2, 0, BOTTOM_DIVIDER);

    wrefresh(win);
}

void highlight_on_index(Menu* menu, int index)
{
    for (int i = 0; i < menu->commandsSize; i++)
    {
        if (i == index)
            menu->commands[i].highlight = TRUE;
        else
            menu->commands[i].highlight = FALSE;
    }
}
