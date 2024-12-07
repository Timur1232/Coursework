#include "proccess_fec.h"

#include <macros.h>
#include "../fec_note/fec_note.h"
#include "../list/list.h"
#include "../ref_array/ref_array.h"

float calc_diff_deviation(FECNotePtr note)
{
    return note->energyConsReal - note->energyConsPlan;
}

FECNote calc_energy_sum(ListPtr list)
{
    FECNote sum = init_note();
    for (FOR_RANGE(iter, *list))
    {
        sum.energyConsPlan += iter->data.energyConsPlan;
        sum.energyConsReal += iter->data.energyConsReal;
    }
    return sum;
}

FECNote calc_energy_sum_ref(RefArrayPtr array)
{
    FECNote sum = init_note();
    FECNote** iter = (FECNote**)get_ref(array, 0);
    for (int i = 0; i < array->size; i++, iter++)
    {
        sum.energyConsPlan += (*iter)->energyConsPlan;
        sum.energyConsReal += (*iter)->energyConsReal;
    }
    return sum;
}
