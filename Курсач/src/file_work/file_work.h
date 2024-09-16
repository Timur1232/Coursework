#ifndef FILE_WORK_H
#define FILE_WORK_H

#include "../fec_note/fec_note.h"
#include "../list/list.h"

void scan_note(const char* fileName, FECNote* note);
void scan_note_list(const char* fileName, ListPtr fecNotes);

#endif