#include "interface.h"

#include <string.h>

#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "../proccess_fec/proccess_fec.h"

static const wchar_t* const TOP_DIVIDER =      L"┌───────┬──────────────┬──────────────┬─────────────────┬─────────────────┬─────────────┬─────────────┬─────────────┐";
static const wchar_t* const MIDDLE_DIVIDER =   L"├───────┼──────────────┼──────────────┼─────────────────┼─────────────────┼─────────────┼─────────────┼─────────────┤";
static const wchar_t* const BOTTOM_DIVIDER =   L"└───────┴──────────────┴──────────────┴─────────────────┴─────────────────┴─────────────┴─────────────┴─────────────┘";

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

static void print_vert_borders(WINDOW* win, int posY)
{
    const wchar_t* ch = L"│";
    mvwaddwstr(win, posY, 0, ch);
    mvwaddwstr(win, posY, 8, ch);
    mvwaddwstr(win, posY, 23, ch);
    mvwaddwstr(win, posY, 38, ch);
    mvwaddwstr(win, posY, 56, ch);
    mvwaddwstr(win, posY, 74, ch);
    mvwaddwstr(win, posY, 88, ch);
    mvwaddwstr(win, posY, 102, ch);
    mvwaddwstr(win, posY, 116, ch);
}

static void print_border(WINDOW* win)
{
    mvwaddwstr(win, 0, 0, TOP_DIVIDER);
    mvwaddwstr(win, 1, 0, L"│   #   │  Сер. номер  │ Номер завода │  ФИО директора  │  ФИО инженера   │    План     │   Расход    │ Отклонение  │");
    for (int i = 0; i < CHUNCK_SIZE; i++)
    {
        mvwaddwstr(win, i * 2 + 2, 0, MIDDLE_DIVIDER);
        print_vert_borders(win, i * 2 + 3);
    }
    mvwaddwstr(win, CHUNCK_SIZE * 2 + 2, 0, BOTTOM_DIVIDER);
}

static void print_sum(WINDOW* win, FECNote sumNote)
{
    const wchar_t* ch = L"│";
    int posY = CHUNCK_SIZE * 2 + 4;
    mvwaddwstr(win, posY - 1, 56, L"┌─────────────────┬─────────────┬─────────────┬─────────────┐");
    mvwprintw(win, posY, 74, "  %-11.2f   %-11.2f   %11.2f  ",
        sumNote.energyConsPlan, sumNote.energyConsReal, calc_diff_deviation(&sumNote)
    );
    mvwaddwstr(win, posY, 56, L"│      Сумма      │");
    mvwaddwstr(win, posY, 88, ch);
    mvwaddwstr(win, posY, 102, ch);
    mvwaddwstr(win, posY, 116, ch);
    mvwaddwstr(win, posY + 1, 56, L"└─────────────────┴─────────────┴─────────────┴─────────────┘");
}

void print_table_list(WINDOW* win, ListPtr list, int chunck)
{
    wclear(win);

    Iterator iter = get_iter(list, chunck * CHUNCK_SIZE);

    for (int i = 0; i < CHUNCK_SIZE && (i + CHUNCK_SIZE * chunck) < list->size; i++, INCREMENT(iter))
    {
        mvwprintw(win, i * 2 + 3, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * CHUNCK_SIZE + 1, iter->data.serialNumber, iter->data.factoryNumber,
            iter->data.directorFullName, iter->data.engineerFullName,
            iter->data.energyConsPlan, iter->data.energyConsReal,
            calc_diff_deviation(&iter->data)
        );
    }
    print_sum(win, calc_energy_sum(list));

    print_border(win);

    wrefresh(win);
}

void print_table_ref(WINDOW* win, RefArrayPtr entries, int chunck)
{
    wclear(win);

    FECNote** iter = (FECNote**)get_ref(entries, chunck * CHUNCK_SIZE);

    for (int i = 0; i < CHUNCK_SIZE && (i + CHUNCK_SIZE * chunck) < entries->size; i++, iter++)
    {
        mvwprintw(win, i * 2 + 3, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * CHUNCK_SIZE + 1, (*iter)->serialNumber, (*iter)->factoryNumber,
            (*iter)->directorFullName, (*iter)->engineerFullName,
            (*iter)->energyConsPlan, (*iter)->energyConsReal,
            calc_diff_deviation((*iter))
        );
    }
    print_sum(win, calc_energy_sum_ref(entries));
    print_border(win);

    wrefresh(win);
}

void pop_up_notification(const wchar_t* messege, int x, int y)
{
    WINDOW* win = newwin(3, wcslen(messege) + 4, y, x);
    refresh();
    box(win, 0, 0);
    mvwaddwstr(win, 1, 2, messege);
    wrefresh(win);
    delwin(win);
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
