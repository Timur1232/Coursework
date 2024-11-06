#include "undo_stack.h"

#include <types.h>
#include <macros.h>
#include "../log/log.h"
#include "../list/list.h"

UndoStack init_undo()
{
    UndoStack stack = { NULL, NULL, 0, false };
    return stack;
}

UndoNode* new_undo(ActionType action, FECNote* note, int index)
{
    UndoNode* newNode = NEW(UndoNode, 1);
    if (!newNode)
    {
        LOG_DEBUG(LOG_ERR, "undo_stack.c", "new_undo()", "malloc() returned NULL", LOG_FILE);
        exit(-1);
    }

    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->state = *note;
    newNode->index = index;
    newNode->action = action;

    return newNode;
}

void free_undo(UndoStack* stack)
{
    if (stack->size == 0) { return; }
    else if (stack->size == 1) { DELETE(stack->top); }
    else
    {
        UndoNode* iterator = stack->top->next;
        while (iterator->next != NULL)
        {
            DELETE(iterator->prev);
            INCREMENT(iterator);
        }
        DELETE(iterator);
    }
    *stack = init_undo();
}

void push_action(UndoStack* stack, ActionType action, FECNote* state, int index)
{
    pop_action(stack);
    if (stack->size == UNDO_LIMIT)
    {
        UndoNode* iterator = stack->top;
        while (iterator->next) INCREMENT(iterator);
        iterator->prev->next = NULL;
        DELETE(iterator);
    }
    UndoNode* newNode = new_undo(action, state, index);
    stack->size++;

    if (stack->top)
    {
        CONNECT_NODES(newNode, stack->top);
        DECREMENT(stack->top);
    }
    else
    {
        stack->top = newNode;
    }
    stack->cur = newNode;
}

void pop_action(UndoStack* stack)
{
    if (stack->cur == stack->top) { return; }
    if (stack->size <= 1 || stack->end) { free_undo(stack); return; }
    UndoNode* iter = stack->cur->prev;
    int count = 0;

    if (iter->prev)
    {
        DECREMENT(iter);
        DELETE(iter->next);
        count++;
        while (iter->prev)
        {
            DECREMENT(iter);
            DELETE(iter->next);
            count++;
        }
    }
    else
    {
        count = 1;
    }
    DELETE(iter);
    stack->size -= count;
    stack->cur->prev = NULL;
    stack->top = stack->cur;
    stack->end = false;
}

ActionType undo(UndoStack* stack, ListPtr list)
{
    if (!stack->cur || stack->end) { return UNDO_EMPTY; }

    UndoNode* undoAction = stack->cur;
    switch (undoAction->action)
    {
    case UNDO_DEL:
        insert(list, &undoAction->state, undoAction->index);
        break;
    case UNDO_ADD:
        pop(list, undoAction->index);
        break;
    case UNDO_CHANGE:
        *get_at(list, undoAction->index) = undoAction->state;
        break;
    case UNDO_CLEAR:
        return UNDO_CLEAR;
    default:
        LOG_DEBUG(LOG_ERR, "undo_stack.c", "undo()", "Unexpected action", LOG_FILE);
        exit(-1);
    }

    if (stack->cur->next)
    {
        INCREMENT(stack->cur);
    }
    else
    {
        stack->end = true;
    }
    return undoAction->action;
}

ActionType redo(UndoStack* stack, ListPtr list)
{
    if (!stack->cur || (!stack->cur->prev && !stack->end)) { return UNDO_EMPTY; }

    UndoNode* redoAction;
    if (stack->end)
    {
        redoAction = stack->cur;
    }
    else
    {
        redoAction = stack->cur->prev;
    }

    switch (redoAction->action)
    {
    case UNDO_DEL:
        pop(list, redoAction->index);
        break;
    case UNDO_ADD:
        insert(list, &redoAction->state, redoAction->index);
        break;
    case UNDO_CHANGE:
        *get_at(list, redoAction->index) = redoAction->state;
        break;
    case UNDO_CLEAR:
        return UNDO_CLEAR;
    default:
        LOG_DEBUG(LOG_ERR, "undo_stack.c", "redo()", "Unexpected action", LOG_FILE);
        exit(-1);
    }

    if (stack->end)
    {
        stack->end = false;
    }
    else if (stack->cur->prev)
    {
        DECREMENT(stack->cur);
    }
    return redoAction->action;
}
