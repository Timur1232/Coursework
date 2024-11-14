#ifndef MACROS_H
#define MACROS_H

#include <stdlib.h>

#define clamp(num, min, max) (((num) > (max)) ? (max) : (((num) < (min)) ? (min) : (num)))
#define loop(num, min, max) (((num) > (max)) ? (min) : (((num) < (min)) ? (max) : (num)))

//____________________________[Динамическая память]____________________________//

#define NEW(type, size) (type*)malloc(sizeof(type) * size)
#define DELETE(mem) free(mem)

//__________________________________[Список]__________________________________//

#define CONNECT_NODES(node1, node2) { node1->next = node2; node2->prev = node1; }

#define SWAP(node1, node2) { 		\
    value_t temp = node1->data;		\
    node1->data = node2->data;		\
    node2->data = temp;				\
}

#define INCREMENT(node) node = node->next
#define DECREMENT(node) node = node->prev

#define FOR_RANGE(iter, list) Iterator iter = (list).begin; iter != NULL; INCREMENT(iter)
#define FOR_RANGE_BACK(iter, list) Iterator iter = (list).end; iter != NULL; DECREMENT(iter)

#endif // MACROS_H