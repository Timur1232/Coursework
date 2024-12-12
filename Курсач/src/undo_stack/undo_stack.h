#ifndef UNDO_STACK_H
#define UNDO_STACK_H

#include <types.h>
#include "../fec_note/fec_note.h"
#include "../list/list.h"

#define UNDO_LIMIT 100

typedef enum ActionType
{
    UNDO_DEL,
    UNDO_ADD,
    UNDO_CHANGE,

    UNDO_EMPTY
} ActionType;

typedef struct UndoNode
{
    FECNote state;
    int index;
    ActionType action;
    struct UndoNode* next;
    struct UndoNode* prev;
} UndoNode;

typedef struct UndoStack
{
    UndoNode* top;
    UndoNode* cur;
    int size;
    Bool end;
} UndoStack;

UndoStack init_undo();
UndoNode* new_undo(ActionType action, FECNote* state, int index);
void free_undo(UndoStack* stack);

/* *
   * ������� ��� �������� ���� cur
   * ��������� �������� ����� �������� �� ������� ��������� cur
*/
void push_action(UndoStack* stack, ActionType action, FECNote* state, int index);

// * ������� ��� �������� ���� cur
void pop_action(UndoStack* stack);

// * ���������� ��������� cur ����
ActionType undo(UndoStack* stack, ListPtr list);
// * ���������� ��������� cur �����
ActionType redo(UndoStack* stack, ListPtr list);

#endif // UNDO_STACK_H