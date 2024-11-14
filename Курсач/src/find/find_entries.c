#include "find_entries.h"

#include <stdio.h>

#include "../ref_array/ref_array.h"
#include "../list/list.h"

void find_entries(ListPtr fecNotes, RefArrayPtr entries, FECNotePtr required, int (*compare)(ConstValuePtr val1, ConstValuePtr val2))
{
    clear_array(entries);
    for (FOR_RANGE(iter, *fecNotes))
    {
        if (compare(required, &iter->data) == 0)
        {
            add_ref(entries, &iter->data);
        }
    }
}
