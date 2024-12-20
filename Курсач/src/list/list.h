#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <macros.h>
#include <types.h>
#include "..\fec_note\fec_note.h"

//_____________________________[Предопределения типов]_____________________________//

typedef struct FECNote value_t;

typedef value_t* ValuePtr;
typedef const value_t* ConstValuePtr;

struct ListNode;
struct List;

typedef struct ListNode ListNode;
typedef struct List List;

typedef ListNode* ListNodePtr;
typedef List* ListPtr;

typedef ListNodePtr Iterator;

//__________________________________[Прототипы]__________________________________//

ListNodePtr new_node(ConstValuePtr data);

List init_list();

void copy_list(ListPtr from, ListPtr to);
void free_list(ListPtr list);

// Добавление элементов в список
void push_back(ListPtr list, ConstValuePtr data);
void push_front(ListPtr list, ConstValuePtr data);
int insert(ListPtr list, ConstValuePtr data, uint_64 pos);

// Удаление элементов в список
void pop_back(ListPtr list);
void pop_front(ListPtr list);
int pop(ListPtr list, uint_64 pos);

// Доступ к элементам
Iterator get_iter(ListPtr list, uint_64 pos);
ValuePtr get_at(ListPtr list, uint_64 pos);

// Сортировка
void sort_asc(ListPtr list, int (*compare)(ConstValuePtr val1, ConstValuePtr val2));
void sort_desc(ListPtr list, int (*compare)(ConstValuePtr val1, ConstValuePtr val2));

void swap(ListPtr list, uint_64 pos1, uint_64 pos2);

//__________________________________[Структуры]__________________________________//

struct ListNode
{
    value_t data;
    ListNodePtr next;
    ListNodePtr prev;
};

struct List
{
    ListNodePtr begin;
    ListNodePtr end;
    uint_64 size;
};

#endif // DOUBLE_LIST_H