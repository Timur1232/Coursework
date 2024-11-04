#include "coursework.h"

#include <stdio.h>

#include <types.h>
#include <macros.h>
#include "list/list.h"
#include "ref_array/ref_array.h"
#include "interface/interface.h"

static const MenuCommand mainCom[] = {
    {L" Новый список     ", TRUE, new_list},
    {L" Загрузить список ", FALSE, load_list},
    //{L"Настройки", FALSE, settings}
};

static const Menu mainMenu = {
    (SCREEN_HEIGHT / 2 - 5), -7, 1, 1,
    L" Программа ",
    L" Выход     ",
    mainCom,
    MIDDLE
};

static const MenuCommand browsingCom[] = {
    {L" Сохранить         ", TRUE, new_list},
    {L" Сортировка        ", FALSE, load_list},
    {L" Найти             ", FALSE, find},
    {L" Сохранить расчеты ", FALSE, save_result}
};

static const Menu browsingMenu = {
    3, 0, 1, 1,
    L" Редактор ",
    L" Выход    ",
    browsingCom,
    LEFT
};


ProgramInstance init_program()
{
    ProgramInstance program;

    // Список и массив вхождений
    program.fecNotes = init_list();
    program.entries = init_ref_array(0);

    // Создание окон для дальнейшего вывода
    program.menuWin = newwin(SCREEN_HEIGHT, SCREEN_WIDTH, 0, 0);
    program.browsingWin = newwin(
        BROWSING_MENU_WIN_HEIGHT, BROWSING_MENU_WIN_WIDTH,
        0, 0
    );
    program.tableWin = newwin(
        TABLE_WIN_HEIGHT, TABLE_WIN_WIDTH,
        TABLE_WIN_Y, TABLE_WIN_X
    );
    program.redactorWin = newwin(
        REDACTOR_MENU_WIN_HEIGHT, REDACTOR_MENU_WIN_WIDTH,
        REDACTOR_MENU_WIN_Y, REDACTOR_MENU_WIN_X
    );
    program.popUpWin = newwin(5, 15, 0, 0);

    // Инициализация меню
    program.mainMenu = mainMenu;
    program.browsingMenu = browsingMenu;

    return program;
}

int Main(int argc, char** argv)
{
    // Инициализация PDCurses
    initscr();
    noecho();
    //curs_set(0);
    //mouse_set(ALL_MOUSE_EVENTS);
    keypad(stdscr, TRUE);
    //timeout(32);
    resize_term(40, 136);
    if (has_colors())
        start_color();

    ProgramInstance program = init_program();



    endwin();
    return 0;
}
