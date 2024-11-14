#ifndef REF_ARRAY_H
#define REF_ARRAY_H

#include <types.h>

struct RefArray;

typedef struct RefArray RefArray;
typedef RefArray* RefArrayPtr;

/**
* Динамический массив ссылок
* 
*/
struct RefArray
{
    void** data;
    uint_64 size;
    uint_64 capacity;
};

RefArray init_ref_array(uint_64 reserve);
void add_ref(RefArrayPtr array, void* ref);
void clear_array(RefArrayPtr array);
void free_array(RefArrayPtr array);

void** get_ref(RefArrayPtr array, uint_64 pos);

#endif // REF_ARRAY_H