#include "find_entries.h"

#include <stdio.h>

#include "../ref_array/ref_array.h"
#include "../list/list.h"

RefArray find_entries(ListPtr fecNotes, FECNotePtr required, int (*compare)(ConstValuePtr val1, ConstValuePtr val2))
{
    RefArray entries = init_ref_array(128);

    for (FOR_RANGE(iter, *fecNotes))
    {
        if (compare(&iter->data, required) == 0)
        {
            add_ref(&entries, &iter->data);
        }
    }
    puts("");

    return entries;
}

void print_entries(RefArrayPtr entries)
{
    if (!entries->size)
    {
        puts("Вхождений не найдено");
        return;
    }

    puts("-----------------------------------------------------------------------");
    puts("| #   | #Зв | ФИО директора   | ФИО инженера    | План     | Расход   |");
    puts("-----------------------------------------------------------------------");

    FECNote** iter = (FECNote**)entries->data;
    for (uint_64 i = 0; i < entries->size; i++, iter++)
    {
        //print_note(*iter);
    }
    puts("-----------------------------------------------------------------------");
}
