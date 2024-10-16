#include "proccess_fec.h"

#include <macros.h>
#include "../fec_note/fec_note.h"
#include "../list/list.h"

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
