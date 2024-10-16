#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include <curses.h>

#include <types.h>
#include "../list/list.h"
#include "../ref_array/ref_array.h"

typedef enum Align
{
    LEFT,
    MIDDLE
} Align;

typedef struct MenuCommand
{
    const wchar_t* text;
    //int hitBoxWidth;
    //int hitBoxShiftX;
    Bool highlight;
    void (*function)();
} MenuCommand;

typedef struct Menu
{
    int commandStartY;
    int commandShiftX;
    const wchar_t* title;
    MenuCommand* commands;
    uint_64 commandsSize;
    Align align;
} Menu;

void print_menu(WINDOW* win, Menu* menu);
void print_table_list(WINDOW* win, ListPtr list, int num, int chunck);
void print_table_ref(WINDOW* win, RefArrayPtr entries, int num, int chunck);

void highlight_on_index(Menu* menu, int index);
void highlight_on_hover(Menu* menu);

void print_main_screen();
void print_editor_screen();


#endif // CONSOLE_INTERFACE_H