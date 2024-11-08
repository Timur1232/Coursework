#include "interface.h"

#include <string.h>
#include <curses.h>

#include <macros.h>
#include "../list/list.h"
#include "../fec_note/fec_note.h"
#include "../proccess_fec/proccess_fec.h"
#include "../log/log.h"
#include "../user_input/user_input.h"

static const wchar_t* const TOP_DIVIDER =      L"┌───────┬──────────────┬──────────────┬─────────────────┬─────────────────┬─────────────┬─────────────┬─────────────┐";
static const wchar_t* const MIDDLE_DIVIDER =   L"├───────┼──────────────┼──────────────┼─────────────────┼─────────────────┼─────────────┼─────────────┼─────────────┤";
static const wchar_t* const BOTTOM_DIVIDER =   L"└───────┴──────────────┴──────────────┴─────────────────┴─────────────────┴─────────────┴─────────────┴─────────────┘";

static const wchar_t* const NOTE_REDACTOR_FIELDS[6] = {
    L" Серийный номер ",
    L" Номер завода   ",
    L" ФИО директора  ",
    L" ФИО инженера   ",
    L" План           ",
    L" Расход         "
};

static void print_vert_borders(WINDOW* win, int posY);
static void print_border(WINDOW* win, int startY, int chunchSize);
static void print_sum(WINDOW* win, FECNote sumNote);
static void print_table_info(WINDOW* win, int chunck, int index, int size, int chunckSize);
static void highlight_elem(WINDOW* win, int posY);
static char* input_str(WINDOW* win, int x, int y);

void print_menu(WINDOW* win, const Menu* menu)
{
    wclear(win);
    box(win, 0, 0);

    int startX = (menu->align == MIDDLE) ? (getmaxx(win) / 2) : 2;

    // Вывод заголовка
    if (menu->title)
        mvwaddwstr(win,
            menu->commandStartY + menu->titleShiftY - 1,
            startX - ((menu->align == MIDDLE) ? (wcslen(menu->title) / 2) : 0),
            menu->title);

    // Вывод пунктов
    int count = 0;
    for (int i = 0; i < menu->commandsSize; i++)
    {
        if (menu->selected == i)    // Выделение активного пункта
        {
            wattron(win, WA_REVERSE);
            count++;
        }
        mvwaddwstr(win,
            menu->commandStartY + i,
            startX + menu->commandShiftX,
            menu->commands[i].text
        );
        if (menu->selected == i)
        {
            wattroff(win, WA_REVERSE);
        }
    }

    if (menu->exitText)
    {
        if (!count)
        {
            wattron(win, WA_REVERSE);
        }
        mvwaddwstr(win,
            menu->commandStartY + menu->commandsSize - 1 + menu->exitTextShiftY,
            startX + menu->commandShiftX,
            menu->exitText
        );
        if (!count)
        {
            wattroff(win, WA_REVERSE);
        }
    }

    wrefresh(win);
}

void print_table_list(WINDOW* win, ListPtr list, int selected, TableMode mode)
{
    wclear(win);

    int chunckSize = (mode == FULL) ? (CHUNCK_SIZE_FULL) : (CHUNCK_SIZE_REDACTOR);
    int chunck = selected / chunckSize;
    int startY = (mode == FULL) ? (0) : (14);

    Iterator iter = get_iter(list, chunck * chunckSize);
    // Вывод блока элементов списка
    for (int i = 0; i < chunckSize && (i + chunckSize * chunck) < list->size; i++, INCREMENT(iter))
    {
        if (i == selected % chunckSize)     // Выделение активного элемента
        {
            wattron(win, WA_REVERSE);
        }
        mvwprintw(win, i * 2 + 3 + startY, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * chunckSize + 1, iter->data.serialNumber, iter->data.factoryNumber,
            iter->data.directorFullName, iter->data.engineerFullName,
            iter->data.energyConsPlan, iter->data.energyConsReal,
            calc_diff_deviation(&iter->data)
        );
        if (i == selected % chunckSize)
        {
            wattroff(win, WA_REVERSE);
        }
    }
    print_sum(win, calc_energy_sum(list));
    print_table_info(win, chunck, selected, list->size, chunckSize);
    print_border(win, startY, chunckSize);
    if ((chunck + 1) * chunckSize > list->size)
    {
        if (selected == list->size)
            wattron(win, WA_REVERSE);
        mvwprintw(win, list->size % chunckSize * 2 + 3 + startY, 1, "   +   ");
        mvwaddwstr(win, list->size % chunckSize * 2 + 3 + startY, 9, L" Новый        ");
        wattroff(win, WA_REVERSE);
    }
    highlight_elem(win, (selected % chunckSize) * 2 + startY);

    wrefresh(win);
}

void print_table_ref(WINDOW* win, RefArrayPtr entries, int selected, TableMode mode)
{
    wclear(win);

    int chunckSize = (mode == FULL) ? (CHUNCK_SIZE_FULL) : (CHUNCK_SIZE_REDACTOR);
    int chunck = selected / chunckSize;
    int startY = (mode == FULL) ? (0) : (14);

    FECNote** iter = (FECNote**)get_ref(entries, chunck * chunckSize);
    // Вывод блока элементов массива ссылок
    for (int i = 0; i < chunckSize && (i + chunckSize * chunck) < entries->size; i++, iter++)
    {
        if (i == selected % chunckSize)
        {
            wattron(win, WA_REVERSE);
        }
        mvwprintw(win, i * 2 + 3 + startY, 0, "  %-5d   %-12d   %-12d   %-15s   %-15s   %-11.2f   %-11.2f   %11.2f  ",
            i + chunck * chunckSize + 1, (*iter)->serialNumber, (*iter)->factoryNumber,
            (*iter)->directorFullName, (*iter)->engineerFullName,
            (*iter)->energyConsPlan, (*iter)->energyConsReal,
            calc_diff_deviation((*iter))
        );
        if (i == selected % chunckSize)
        {
            wattroff(win, WA_REVERSE);
        }
    }
    print_sum(win, calc_energy_sum_ref(entries));
    print_table_info(win, chunck, selected, entries->size, chunckSize);
    print_border(win, startY, chunckSize);
    highlight_elem(win, (selected % chunckSize) * 2 + startY);

    wrefresh(win);
}

// TODO: расчет позиции
void pop_up_notification_wchar(WINDOW* win, const wchar_t* message, NotificationType type, int y)
{
    int messageLength = wcslen(message);
    wclear(win);
    wresize(win, 3, messageLength + 4);
    mvwin(win, y, SCREEN_WIDTH / 2 - messageLength / 2);
    wbkgd(win, COLOR_PAIR(type));
    box(win, 0, 0);
    mvwaddwstr(win, 1, 2, message);
    wrefresh(win);
    wbkgd(win, COLOR_PAIR(0));
}

void pop_up_notification(WINDOW* win, const char* message, NotificationType type, int y)
{
    int messageLength = strlen(message);
    wclear(win);
    wresize(win, 3, messageLength + 4);
    mvwin(win, y, SCREEN_WIDTH / 2 - messageLength / 2);
    wbkgd(win, COLOR_PAIR(type));
    box(win, 0, 0);
    mvwaddstr(win, 1, 2, message);
    wrefresh(win);
    wbkgd(win, COLOR_PAIR(0));
}

// nodiscard
char* get_user_input_str(WINDOW* win, const wchar_t* message)
{
    wclear(win);
    keypad(win, TRUE);
    wresize(win, 4, 60);
    mvwin(win, SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2 - 30);
    box(win, 0, 0);
    mvwaddwstr(win, 1, 2, message);
    wrefresh(win);
    //echo();
    curs_set(1);
    char* str = input_str(win, 2, 2);
    curs_set(0);
    //noecho();
    return str;
}

int get_user_input_int(WINDOW* win, const wchar_t* message, int* dest)
{
    char* str = get_user_input_str(win, message);
    int err = parse_int(str, dest);
    free(str);
    return err;
}

int get_user_input_float(WINDOW* win, const wchar_t* message, float* dest)
{
    char* str = get_user_input_str(win, message);
    int err = parse_float(str, dest);
    free(str);
    return err;
}

void print_note_editor(WINDOW* win, FECNotePtr note, int field, int index)
{
    wclear(win);
    box(win, 0, 0);

    int posX = 2;
    int posY = 2;

    mvwaddwstr(win, posY, posX, L"Запись №");
    mvwprintw(win, posY, posX + 8, "%d", index + 1);
    mvwaddwstr(win, posY + 2, 0, L"├───────────────────────────────────────────────────────────────────────────────────────────────────────────────────┤");

    // Вывод названий полей
    posY += 3;
    for (int i = 0; i < 6; i++)
    {
        if (field == i) wattron(win, WA_REVERSE);
        mvwaddwstr(win, posY + i, posX, NOTE_REDACTOR_FIELDS[i]);
        if (field == i) wattroff(win, WA_REVERSE);
    }
    mvwaddwstr(win, posY + 7, posX, L"Отклонение плана");
    mvwaddwstr(win, posY + 9, 0, L"├───────┬──────────────┬──────────────┬─────────────────┬─────────────────┬─────────────┬─────────────┬─────────────┤");

    // Вывод значений полей структуры
    if (note)
    {
        posX += 19;
        mvwprintw(win, posY, posX, "%d", note->serialNumber);
        mvwprintw(win, posY + 1, posX, "%d", note->factoryNumber);
        mvwprintw(win, posY + 2, posX, "%s", note->directorFullName);
        mvwprintw(win, posY + 3, posX, "%s", note->engineerFullName);
        mvwprintw(win, posY + 4, posX, "%.2f", note->energyConsPlan);
        mvwprintw(win, posY + 5, posX, "%.2f", note->energyConsReal);
        mvwprintw(win, posY + 7, posX, "%.2f", calc_diff_deviation(note));
    }

    wrefresh(win);
}

//void highlight_on_index(const Menu* menu, int index)
//{
//    for (int i = 0; i < menu->commandsSize; i++)
//    {
//        if (i == index)
//            menu->commands[i].highlight = TRUE;
//        else
//            menu->commands[i].highlight = FALSE;
//    }
//}

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

static void print_border(WINDOW* win, int startY, int chunchSize)
{
    mvwaddwstr(win, startY, 0, TOP_DIVIDER);
    mvwaddwstr(win, startY + 1, 0, L"│   #   │  Сер. номер  │ Номер завода │  ФИО директора  │  ФИО инженера   │    План     │   Расход    │ Отклонение  │");
    for (int i = 0; i < chunchSize; i++)
    {
        mvwaddwstr(win, i * 2 + 2 + startY, 0, MIDDLE_DIVIDER);
        print_vert_borders(win, i * 2 + 3 + startY);
    }
    mvwaddwstr(win, chunchSize * 2 + 2 + startY, 0, BOTTOM_DIVIDER);
}

static void print_sum(WINDOW* win, FECNote sumNote)
{
    const wchar_t* ch = L"│";
    int posY = 36;
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

static void print_table_info(WINDOW* win, int chunck, int index, int size, int chunckSize)
{
    const wchar_t* ch = L"│";
    int posY = 35;
    mvwaddwstr(win, posY, 2, L"Страница:");
    mvwprintw(win, posY, 12, "%d/%d", chunck + 1, size / chunckSize + 1);
    mvwaddwstr(win, posY, 23, L"Элемент:");
    mvwprintw(win, posY, 32, "%d/%d", index + 1, size);

    mvwaddwstr(win, posY + 2, 2, L"Листать: ← →");
    mvwaddwstr(win, posY + 2, 23, L"Выбор: ↑ ↓");

}

static void highlight_elem(WINDOW* win, int posY)
{
    wattron(win, WA_BOLD);
    mvwaddwstr(win, posY + 2, 0, L"├=======┼==============┼==============┼=================┼=================┼=============┼=============┼=============┤");
    mvwaddwstr(win, posY + 3, 0, L"│");
    mvwaddwstr(win, posY + 3, 116, L"│");
    if (posY == 30)
        mvwaddwstr(win, posY + 4, 0, L"└=======┴==============┴==============┴=================┴=================┴=============┴=============┴=============┘");
    else
        mvwaddwstr(win, posY + 4, 0, L"├=======┼==============┼==============┼=================┼=================┼=============┼=============┼=============┤");
    wattroff(win, WA_BOLD);
}

static char* input_str(WINDOW* win, int x, int y)
{
    char* buff = NEW(char, BUFFER_CAPASITY);
    if (!buff)
    {
        LOG_DEBUG(ERR, "user_input.c", "input_str()", "buff allocation error", LOG_FILE);
        exit(-1);
    }
    memset(buff, 0, BUFFER_CAPASITY);
    int size = 0;
    int capasity = BUFFER_CAPASITY;

    wmove(win, y, x);
    int ch = 0;
    while (true)
    {
        ch = wgetch(win);
        if (ch == '\n')
        {
            break;
        }
        if (ch == 8)
        {
            size = clamp(size - 1, 0, capasity);
            buff[size] = '\0';
        }
        if (size == capasity - 1)
        {
            capasity += BUFFER_CAPASITY;
            char* reallocBuff = realloc(buff, capasity);
            if (!reallocBuff)
            {
                free(buff);
                LOG_DEBUG(LOG_ERR, "user_input.c", "input_str()", "buff reallocation error", LOG_FILE);
                exit(-1);
            }
            buff = reallocBuff;
        }
        if (ch != 8)
        {
            buff[size++] = ch;
            buff[size] = '\0';
        }
        char* outStr = buff;
        mvwprintw(win, y, x, "                                                        ");
        if (size > getmaxx(win) - 4)
        {
            outStr = &buff[size - (getmaxx(win) - 7)];
            mvwprintw(win, y, x, "...");
            mvwprintw(win, y, x + 3, outStr);
        }
        else
        {
            mvwprintw(win, y, x, outStr);
        }
        wrefresh(win);
    }
    return buff;
}