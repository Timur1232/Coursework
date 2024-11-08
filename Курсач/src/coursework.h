#ifndef COURSEWORK_H
#define COURSEWORK_H

#include <curses.h>

#include <types.h>
#include "list/list.h"
#include "ref_array/ref_array.h"
#include "undo_stack/undo_stack.h"

typedef struct ProgramInstance
{
    WINDOW* winMain;
    WINDOW* winTable;
    WINDOW* winRed;
    WINDOW* winMenu;
    WINDOW* popUp;

    char* currentFilePath;
    List fecNotes;
    RefArray entries;
    UndoStack undoStack;
    FECNote copyNote;

    int selectedNode;
    int field;
    int tableMode;
    Bool findMode;
    int chunckSize;
    Bool copied;
} ProgramInstance;

typedef void (*StructuralFunc) (ProgramInstance* program);

ProgramInstance init_program();

int Main(int argc, char** argv);

void new_list(ProgramInstance* program);
void load_list(ProgramInstance* program);
void list_redactor(ProgramInstance* program);
//void settings(ProgramInstance* program);

void save(ProgramInstance* program);
void sorting(ProgramInstance* program);
void find(ProgramInstance* program);
void save_result(ProgramInstance* program);


#endif // COURSEWORK_H