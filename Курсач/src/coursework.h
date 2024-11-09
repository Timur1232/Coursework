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
    WINDOW* winField;
    WINDOW* popUp;
    WINDOW* winControls;

    char* currentFileName;
    List fecNotes;
    RefArray entries;
    UndoStack undoStack;
    FECNote copyNote;

    int selectedNode;
    int field;
    Bool findMode;
    int sortMode;
    int chunckSize;
    Bool copied;
    int focus;
    Bool shouldClose;
    Bool saved;
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

int change_serialNumber(ProgramInstance* program, FECNote* note);
int change_factoryNumber(ProgramInstance* program, FECNote* note);
int change_directorFullName(ProgramInstance* program, FECNote* note);
int change_engineerFulName(ProgramInstance* program, FECNote* note);
int change_energyConsPlan(ProgramInstance* program, FECNote* note);
int change_energyConsReal(ProgramInstance* program, FECNote* note);

void add_note(ProgramInstance* program, FECNote* note);
void delete_note(ProgramInstance* program);

#endif // COURSEWORK_H