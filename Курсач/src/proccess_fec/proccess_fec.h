#ifndef PROCCESS_FEC_H
#define PROCCESS_FEC_H

#include "../fec_note/fec_note.h"
#include "../list/list.h"
#include "../ref_array/ref_array.h"

float calc_diff_deviation(FECNotePtr note);
FECNote calc_energy_sum(ListPtr list);
FECNote calc_energy_sum_ref(RefArrayPtr array);

#endif // PROCCESS_FEC_H