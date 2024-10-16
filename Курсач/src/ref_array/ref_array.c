#include "ref_array.h"

#include <stdlib.h>

#include <macros.h>
#include <types.h>
#include "..\log\log.h"
#include "..\list\list.h"

RefArray init_ref_array(uint_64 reserve)
{
    reserve = abs(reserve);
    RefArray array;
    array.size = 0;
    array.capacity = reserve;
    if (reserve > 0)
    {
        array.data = NEW(void*, reserve);
    }
    else
    {
        array.data = NULL;
    }
    return array;
}

void add_ref(RefArrayPtr array, void* ref)
{
    if (!array) { return; }
    
    if (array->size == array->capacity)
    {
        if (array->capacity <= 1) array->capacity = 2;
        else array->capacity += array->capacity / 2;	// +50%

        if (array->data)
        {
            void** reallocBlock = (void**)realloc(array->data, sizeof(void*) * array->capacity);
            if (!reallocBlock)
            {
                LOG(ERR, "ref_array.c", "add_ref()", "realloc() returned NULL", LOG_FILE);
                exit(-1);
            }
            array->data = reallocBlock;
        }
        else
        {
            void** reallocBlock = NEW(void*, array->capacity);
            if (!reallocBlock)
            {
                LOG(ERR, "ref_array.c", "add_ref()", "malloc() returned NULL", LOG_FILE);
                exit(-1);
            }
            array->data = reallocBlock;
        }
    }
    array->data[array->size++] = ref;
}

void clear_array(RefArrayPtr array)
{
    if (!array) { return; }

    if (array->data)
        DELETE(array->data);
    array->size = 0;
    array->capacity = 0;
}

void* get_ref(RefArrayPtr array, uint_64 pos)
{
    if (pos < 0 || pos >= array->size) { return NULL; }
    return &array->data[pos];
}
