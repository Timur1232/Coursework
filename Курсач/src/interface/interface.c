#include "interface.h"

#include <string.h>
#include <curses.h>

#include <macros.h>
#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "../proccess_fec/proccess_fec.h"

static const wchar_t* const TOP_DIVIDER =      L"┌───────┬──────────────┬──────────────┬─────────────────┬─────────────────┬─────────────┬─────────────┬─────────────┐";
static const wchar_t* const MIDDLE_DIVIDER =   L"├───────┼──────────────┼──────────────┼─────────────────┼─────────────────┼─────────────┼─────────────┼─────────────┤";
static const wchar_t* const BOTTOM_DIVIDER =   L"└───────┴──────────────┴──────────────┴─────────────────┴─────────────────┴─────────────┴─────────────┴─────────────┘";

static const wchar_t* const NOTE_REDACTOR_FIELDS[5] = {
    L" Номер завода  ",
    L" ФИО директора ",
    L" ФИО инженера  ",
    L" План          ",
    L" Расход        "
};

static void print_vert_borders(WINDOW* win, int posY);
static void print_border(WINDOW* win);
static void print_sum(WINDOW* win, FECNote sumNote);
static void print_table_info(WINDOW* win, int chunck, int index, int size);
static void highlight_elem(WINDOW* win, int index);

void print_menu(WINDOW* win, Menu* menu)
{
    wclear(win);
    box(win, 0, 0);

    int startX = (menu->align == MIDDLE) ? (getmaxx(win) / 2) : 2;

    // Вывод заголовка
    if (menu->title)
        mvwaddwstr(win,
            menu->commandStartY - 2,
            startX - ((menu->align == MIDDLE) ? (wcslen(menu->title) / 2) : 0),
            menu->title);

    // Вывод пунктов
    for (int i = 0; i < menu->commandsSize; i++)
    {
        if (menu->commands[i].highlight)    // Выделение активного пункта
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

void print_table_list(WINDOW* win, ListPtr list, int chunck, int highlight)
{
    wclear(win);

    Iterator iter = get_iter(list, chunck * CHUNCK_SIZE);
    // Вывод блока элементов списка
    for (int i = 0; i < CHUNCK_SIZE && (i + CHUNCK_SIZE * chunck) < list->size; i++, INCREMENT(iter))
    {
        if (i == highlight)     // Выделение активного элемента
        {
            wattron(win, WA_REVERSE);
        }
        mvwprintw(win, i * 2 + 3, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * CHUNCK_SIZE + 1, iter->data.serialNumber, iter->data.factoryNumber,
            iter->data.directorFullName, iter->data.engineerFullName,
            iter->data.energyConsPlan, iter->data.energyConsReal,
            calc_diff_deviation(&iter->data)
        );
        if (i == highlight)
        {
            wattroff(win, WA_REVERSE);
        }
    }
    print_sum(win, calc_energy_sum(list));
    print_table_info(win, chunck, highlight, list->size);
    print_border(win);
    highlight_elem(win, highlight);

    wrefresh(win);
}

void print_table_ref(WINDOW* win, RefArrayPtr entries, int chunck, int highlight)
{
    wclear(win);

    FECNote** iter = (FECNote**)get_ref(entries, chunck * CHUNCK_SIZE);
    // Вывод блока элементов массива ссылок
    for (int i = 0; i < CHUNCK_SIZE && (i + CHUNCK_SIZE * chunck) < entries->size; i++, iter++)
    {
        if (i == highlight)
        {
            wattron(win, WA_REVERSE);
        }
        mvwprintw(win, i * 2 + 3, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * CHUNCK_SIZE + 1, (*iter)->serialNumber, (*iter)->factoryNumber,
            (*iter)->directorFullName, (*iter)->engineerFullName,
            (*iter)->energyConsPlan, (*iter)->energyConsReal,
            calc_diff_deviation((*iter))
        );
        if (i == highlight)
        {
            wattroff(win, WA_REVERSE);
        }
    }
    print_sum(win, calc_energy_sum_ref(entries));
    print_table_info(win, chunck, highlight, entries->size);
    print_border(win);
    highlight_elem(win, highlight);

    wrefresh(win);
}

void pop_up_notification(WINDOW* win, const wchar_t* messege, int x, int y)
{
    wresize(win, 3, wcslen(messege) + 4);
    mvwin(win, y, x);
    //init_pair(3, COLOR_BLUE, COLOR_WHITE);
    //wbkgd(win, COLOR_PAIR(3));
    box(win, 0, 0);
    mvwaddwstr(win, 1, 2, messege);
    wrefresh(win);
}

void print_note_editor(WINDOW* win, FECNotePtr note, int field)
{
    wclear(win);
    box(win, 0, 0);

    int posX = TABLE_WIN_WIDTH / 2 - 15;
    int posY = TABLE_WIN_HEIGHT / 2 - 10;

    mvwaddwstr(win, posY, posX, L"Запись №");
    mvwprintw(win, posY, posX + 8, "%d", note->serialNumber);

    // Вывод названий полей
    posY += 2;
    for (int i = 0; i < 5; i++)
    {
        if (field == i) wattron(win, WA_REVERSE);
        mvwaddwstr(win, posY + i, posX, NOTE_REDACTOR_FIELDS[i]);
        if (field == i) wattroff(win, WA_REVERSE);
    }
    mvwaddwstr(win, posY + 6, posX, L"Отклонение плана");

    // Вывод значений полей структуры
    posX += 19;
    mvwprintw(win, posY, posX, "%d", note->factoryNumber);
    mvwprintw(win, posY + 1, posX, "%s", note->directorFullName);
    mvwprintw(win, posY + 2, posX, "%s", note->engineerFullName);
    mvwprintw(win, posY + 3, posX, "%f", note->energyConsPlan);
    mvwprintw(win, posY + 4, posX, "%f", note->energyConsReal);
    mvwprintw(win, posY + 6, posX, "%f", calc_diff_deviation(note));

    wrefresh(win);
}

void highlight_on_index(Menu* menu, int index, int prev)
{
    menu->commands[index].highlight = TRUE;
    menu->commands[prev].highlight = FALSE;
}

//__________________________________[Статические функции]__________________________________//

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

static void print_table_info(WINDOW* win, int chunck, int index, int size)
{
    const wchar_t* ch = L"│";
    int posY = CHUNCK_SIZE * 2 + 3;
    mvwaddwstr(win, posY, 2, L"Страница:");
    mvwprintw(win, posY, 12, "%d/%d", chunck + 1, size / CHUNCK_SIZE + 1);
    mvwaddwstr(win, posY, 23, L"Элемент:");
    mvwprintw(win, posY, 32, "%d/%d", index + 1 + chunck * CHUNCK_SIZE, size);

    mvwaddwstr(win, posY + 2, 2, L"Листать: ← →");
    mvwaddwstr(win, posY + 2, 23, L"Выбор: ↑ ↓");

}

static void highlight_elem(WINDOW* win, int index)
{
    wattron(win, WA_BOLD);
    mvwaddwstr(win, index * 2 + 2, 0, L"├=======┼==============┼==============┼=================┼=================┼=============┼=============┼=============┤");
    mvwaddwstr(win, index * 2 + 3, 0, L"│");
    mvwaddwstr(win, index * 2 + 3, 116, L"│");
    if (index == CHUNCK_SIZE - 1)
        mvwaddwstr(win, index * 2 + 4, 0, L"└=======┴==============┴==============┴=================┴=================┴=============┴=============┴=============┘");
    else
        mvwaddwstr(win, index * 2 + 4, 0, L"├=======┼==============┼==============┼=================┼=================┼=============┼=============┼=============┤");
    wattroff(win, WA_BOLD);
}