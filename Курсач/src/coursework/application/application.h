#ifndef APPLICATION_H
#define APPLICATION_H

#include <curses.h>

#include <types.h>
#include "../../list/list.h"
#include "../../ref_array/ref_array.h"
#include "../../undo_stack/undo_stack.h"

typedef enum Focus
{
    FOCUS_BROWSING,
    FOCUS_FIND,
    FOCUS_EDITOR,
    FOCUS_MENU
} Focus;

typedef enum Align
{
    LEFT,
    MIDDLE
} Align;

typedef enum NotificationType
{
    N_INFO = 2,
    N_ERR = 3
} NotificationType;

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
    Focus focus;
    Bool shouldClose;
    Bool saved;
} ProgramInstance;

typedef void (*StructuralFunc) (ProgramInstance* program);

typedef struct MenuCommand
{
    const wchar_t* text;
    StructuralFunc function;
} MenuCommand;


typedef struct Menu
{
    int selected;

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

ProgramInstance init_program();

int Main(int argc, char** argv);

void new_list(ProgramInstance* program);
void load_list(ProgramInstance* program);
void list_redactor(ProgramInstance* program);

void save(ProgramInstance* program);
void sorting(ProgramInstance* program);
void find(ProgramInstance* program);
//void clear_list(ProgramInstance* program);

#endif // APPLICATION_H