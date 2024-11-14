#include "fec_note.h"

#include <stdio.h>
#include <string.h>

#include "../log/log.h"
#include "../list/list.h"
#include "../proccess_fec/proccess_fec.h"

FECNote init_note()
{
    FECNote note = {
        0, 0, "", "", 0.0f, 0.0f
    };
    return note;
}