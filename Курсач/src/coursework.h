#ifndef COURSEWORK_H
#define COURSEWORK_H

#include <curses.h>

#include "list/list.h"
#include "ref_array/ref_array.h"

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