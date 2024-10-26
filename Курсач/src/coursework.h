#ifndef COURSEWORK_H
#define COURSEWORK_H

#include <curses.h>

#include "list/list.h"
#include "ref_array/ref_array.h"

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

typedef struct ProgramInstance
{
    WINDOW* menuWin;
    WINDOW* tableWin;
    WINDOW* popUpWin;

    Menu mainMenu;
    Menu redactorMenu;
    Menu settingsMenu;

    List fecNotes;
    int chunckNum;
    
    RefArray entries;
} ProgramInstance;

#endif // COURSEWORK_H