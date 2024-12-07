#ifndef FIND_ENTRIES_H
#define FIND_ENTRIES_H

#include "../ref_array/ref_array.h"
#include "../list/list.h"

void find_entries(ListPtr fecNotes, RefArrayPtr entries, FECNotePtr required, int (*compare)(ConstValuePtr val1, ConstValuePtr val2));

#endif // FIND_ENTRIES_H
