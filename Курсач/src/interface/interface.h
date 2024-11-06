#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include <curses.h>

#include <types.h>
#include "../list/list.h"
#include "../ref_array/ref_array.h"
//#include "../coursework.h"

#define CHUNCK_SIZE_FULL 16
#define CHUNCK_SIZE_REDACTOR 9

#define SCREEN_WIDTH 136
#define SCREEN_HEIGHT 40

#define BROWSING_MENU_WIN_WIDTH 19
#define BROWSING_MENU_WIN_HEIGHT SCREEN_HEIGHT

#define TABLE_WIN_WIDTH 117
#define TABLE_WIN_HEIGHT SCREEN_HEIGHT
#define TABLE_WIN_X 19
#define TABLE_WIN_Y 0

#define REDACTOR_MENU_WIN_WIDTH 117
#define REDACTOR_MENU_WIN_HEIGHT 15
#define REDACTOR_MENU_WIN_X 19
#define REDACTOR_MENU_WIN_Y 0

#define BUFFER_CAPASITY 50

#define MY_KEY_REDO 25
#define MY_KEY_UNDO 26


typedef enum Align
{
    LEFT,
    MIDDLE
} Align;

typedef enum TableMode
{
    FULL,
    REDACTOR
} TableMode;

typedef struct MenuCommand
{
    const wchar_t* text;
    Bool highlight;
    void (*function)();
} MenuCommand;

typedef struct Menu
{
    int commandStartY;
    int commandShiftX;
    int titleShiftY;
    int exitTextShiftY;
    const wchar_t* title;
    const wchar_t* exitText;
    MenuCommand* commands;
    int commandsSize;
    Align align;
} Menu;

void print_menu(WINDOW* win, const Menu* menu);
void print_table_list(WINDOW* win, ListPtr list, int selected, TableMode mode);
void print_table_ref(WINDOW* win, RefArrayPtr entries, int selected, TableMode mode);
void print_note_editor(WINDOW* win, FECNotePtr note, int field, int index);

void pop_up_notification(WINDOW* win, const wchar_t* message);
// nodiscard
char* get_user_input(WINDOW* win, const wchar_t* message);

void highlight_on_index(const Menu* menu, int index);
//void highlight_on_hover(Menu* menu);

//void print_main_screen();
//void print_editor_screen();


#endif // CONSOLE_INTERFACE_H