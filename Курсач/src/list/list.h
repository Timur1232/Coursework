#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include "..\fec_note\fec_note.h"
#include "..\macros.h"

//_____________________________[Предопределения типов]_____________________________//

#ifdef _LIST_DEBUG
	typedef int value_t;
#else
	typedef FECNote value_t;
#endif

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
void clear(ListPtr list);

void push_back(ListPtr list, ConstValuePtr data);
void push_front(ListPtr list, ConstValuePtr data);
int insert(ListPtr list, ConstValuePtr data, int pos);

void pop_back(ListPtr list);
void pop_front(ListPtr list);
int pop(ListPtr list, int pos);

Iterator get_iter(ListPtr list, int pos);
ValuePtr get_at(ListPtr list, int pos);

void swap(ListPtr list, int pos1, int pos2);

// Временные функции
void print_list(ListPtr list);
void print_list_backward(ListPtr list);

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
	int size;
};

#endif // DOUBLE_LIST_H