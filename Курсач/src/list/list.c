#include "list.h"

#include <stdio.h>

#include "../proccess_fec/proccess_fec.h"
#include "../log/log.h"

//__________________________________[Node]__________________________________//

ListNodePtr new_node(ConstValuePtr data)
{
    if (!data) { return NULL; }

    ListNodePtr newNode = NEW(ListNode, 1);
    if (!newNode)
    {
        LOG_DEBUG(LOG_ERR, "list.c", "new_node()", "malloc() returned NULL", LOG_FILE);
        exit(-1);
    }
    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = *data;
    return newNode;
}

//__________________________________[List]__________________________________//

List init_list()
{
    List list = {
        NULL, NULL, 0
    };
    return list;
}

void copy_list(ListPtr from, ListPtr to)
{
    if (!to) { return; }
    if (!from) { return; }

    free_list(to);

    for (FOR_RANGE(iter, *from))
    {
        push_back(to, &iter->data);
    }
}

void free_list(ListPtr list)
{
    if (!list) { return; }

    if (list->size == 0) { return; }
    else if (list->size == 1) { DELETE(list->begin); }
    else if (list->size == 2) { DELETE(list->begin); DELETE(list->end); }
    else
    {
        ListNodePtr iterator = list->begin->next;
        while (iterator != NULL)
        {
            DELETE(iterator->prev);
            INCREMENT(iterator);
        }
        DELETE(list->end);
    }
    list->begin = NULL;
    list->end = NULL;
    list->size = 0;
}

void push_back(ListPtr list, ConstValuePtr data)
{
    if (!list) { return; }
    if (!data) { return; }

    ListNodePtr newNode = new_node(data);
    list->size++;

    if (list->end)
    {
        CONNECT_NODES(list->end, newNode);
        INCREMENT(list->end);
    }
    else
    {
        list->begin = list->end = newNode;
    }
}

void push_front(ListPtr list, ConstValuePtr data)
{
    if (!list) { return; }
    if (!data) { return; }

    ListNodePtr newNode = new_node(data);
    list->size++;

    if (list->begin)
    {
        CONNECT_NODES(newNode, list->begin);
        DECREMENT(list->begin);
    }
    else
    {
        list->begin = list->end = newNode;
    }
}

int insert(ListPtr list, ConstValuePtr data, uint_64 pos)
{
    if (!list) { return -1; }
    if (!data) { return -1; }
    if (pos > list->size || pos < 0)
        { return -1; }

    if (pos == 0)
        push_front(list, data);
    else if (pos == list->size)
        push_back(list, data);
    else
    {
        ListNodePtr newNode = new_node(data);

        ListNodePtr node1 = get_iter(list, pos - 1);
        ListNodePtr node2 = node1->next;

        CONNECT_NODES(node1, newNode);
        CONNECT_NODES(newNode, node2);

        list->size++;
    }
    return 0;
}

Iterator get_iter(ListPtr list, uint_64 pos)
{
    if (!list) { return NULL; }
    if (pos >= list->size || pos < 0)
        { return NULL; }

    Iterator iterator;
    if (pos < list->size / 2)
    {
        iterator = list->begin;
        for (uint_64 i = 0; i < pos; i++) INCREMENT(iterator);
    }
    else
    {
        iterator = list->end;
        pos = list->size - pos - 1;
        for (uint_64 i = 0; i < pos; i++) DECREMENT(iterator);
    }
    return iterator;
}

ValuePtr get_at(ListPtr list, uint_64 pos)
{
    if (!list) { return NULL; }
    return &get_iter(list, pos)->data;
}

void pop_back(ListPtr list)
{
    if (!list) { return; }
    if (list->size == 0) { return; }

    list->size--;
    if (list->end->prev)
    {
        DECREMENT(list->end);
        DELETE(list->end->next);
        list->end->next = NULL;
    }
    else
    {
        free_list(list);
    }
}

void pop_front(ListPtr list)
{
    if (!list) { return; }
    if (list->size == 0) { return; }

    list->size--;
    if (list->begin->next)
    {
        INCREMENT(list->begin);
        DELETE(list->begin->prev);
        list->begin->prev = NULL;
    }
    else
    {
        free_list(list);
    }
}

int pop(ListPtr list, uint_64 pos)
{
    if (!list) { return -1; }
    if (pos >= list->size || pos < 0)
        { return -1; }

    if (pos == 0)
        pop_front(list);
    else if (pos == list->size - 1)
        pop_back(list);
    else
    {
        ListNodePtr delNode = get_iter(list, pos);

        CONNECT_NODES(delNode->prev, delNode->next);
        DELETE(delNode);

        list->size--;
    }
    return 0;
}

void sort_asc(ListPtr list, int (*compare)(ConstValuePtr val1, ConstValuePtr val2))
{
    if (!list) { return; }
    for (FOR_RANGE(i, *list))
    {
        for (Iterator j = list->end; j != i; DECREMENT(j))
        {
            if (compare(&j->data, &j->prev->data) < 0)
            {
                SWAP(j->prev, j);
            }
        }
    }
}

void sort_desc(ListPtr list, int(*compare)(ConstValuePtr val1, ConstValuePtr val2))
{
    if (!list) { return; }
    for (FOR_RANGE(i, *list))
    {
        for (Iterator j = list->end; j != i; DECREMENT(j))
        {
            if (compare(&j->data, &j->prev->data) > 0)
            {
                SWAP(j->prev, j);
            }
        }
    }
}

void swap(ListPtr list, uint_64 pos1, uint_64 pos2)
{
    if (!list) { return; }
    if (pos1 < 0 || pos1 >= list->size || pos2 < 0 || pos2 >= list->size)
        { return; }

    ListNodePtr node1 = get_iter(list, pos1),
                node2 = get_iter(list, pos2);
    SWAP(node1, node2);
}