#ifndef COURSEWORK_H
#define COURSEWORK_H

#include <curses.h>

#include "list/list.h"
#include "ref_array/ref_array.h"
#include "interface/interface.h"


typedef struct ProgramInstance
{
    // Окна
    WINDOW* menuWin;
    WINDOW* browsingWin;
    WINDOW* tableWin;
    WINDOW* redactorWin;
    WINDOW* popUpWin;

    // Меню
    Menu mainMenu;
    Menu browsingMenu;

    const char* const currentFilePath;

    List fecNotes;
    RefArray entries;
} ProgramInstance;

typedef void (*StructuralFunc) (ProgramInstance* program);

ProgramInstance init_program();

int Main(int argc, char** argv);

void new_list(ProgramInstance* program);
void load_list(ProgramInstance* program);
//void settings(ProgramInstance* program);

void save(ProgramInstance* program);
void sort(ProgramInstance* program);
void find(ProgramInstance* program);
void save_result(ProgramInstance* program);


#endif // COURSEWORK_H