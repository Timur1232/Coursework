#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include <curses.h>

#include <types.h>
#include "../list/list.h"
#include "../ref_array/ref_array.h"
//#include "../coursework.h"

#define CHUNCK_SIZE 16

#define SCREEN_WIDTH 136
#define SCREEN_HEIGHT 40

#define TABLE_WIN_WIDTH 117
#define TABLE_WIN_HEIGHT SCREEN_HEIGHT
#define TABLE_WIN_X 19
#define TABLE_WIN_Y 0

#define REDACTOR_MENU_WIN_WIDTH 19
#define REDACTOR_MENU_WIN_HEIGHT SCREEN_HEIGHT
#define REDACTOR_MENU_WIN_X 0
#define REDACTOR_MENU_WIN_Y 0

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
    void (*function)(/*ProgramInstance* program*/);
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
void print_table_list(WINDOW* win, ListPtr list, int chunck, int highlight);
void print_table_ref(WINDOW* win, RefArrayPtr entries, int chunck, int highlight);
void pop_up_notification(WINDOW* win, const wchar_t* messege, int x, int y);
void print_note_editor(WINDOW* win, FECNotePtr note, int field);

void highlight_on_index(Menu* menu, int index, int prev);
//void highlight_on_hover(Menu* menu);

void print_main_screen();
void print_editor_screen();


#endif // CONSOLE_INTERFACE_H