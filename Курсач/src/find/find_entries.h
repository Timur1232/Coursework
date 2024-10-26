#ifndef FIND_ENTRIES_H
#define FIND_ENTRIES_H

#include "../ref_array/ref_array.h"
#include "../list/list.h"

RefArray find_entries(ListPtr fecNotes, FECNotePtr required, int (*compare)(ConstValuePtr val1, ConstValuePtr val2));
void print_entries(RefArrayPtr entries);

#endif // FIND_ENTRIES_H
