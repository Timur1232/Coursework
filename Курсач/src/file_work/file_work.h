#ifndef FILE_WORK_H
#define FILE_WORK_H

#include "../fec_note/fec_note.h"
#include "../list/list.h"

#include <stdio.h>

int scan_note(FILE* file, FECNote* note);
void scan_note_list(const char* fileName, ListPtr fecNotes);

void ignore_line(FILE* file);

#endif