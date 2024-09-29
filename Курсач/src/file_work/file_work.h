#ifndef FILE_WORK_H
#define FILE_WORK_H

#include "../fec_note/fec_note.h"
#include "../list/list.h"
#include "parser/parser.h"

#include <stdio.h>

ParserErrorHandler scan_note(FILE* file, FECNote* note);
ParserErrorHandler scan_note_list(const char* fileName, ListPtr fecNotes);
ParserErrors scan_bin_note_list(const char* fileName, ListPtr fecNotes);

ParserErrors save_note_list(const char* fileName, ListPtr fecNotes);
ParserErrors save_bin_note_list(const char* fileName, ListPtr fecNotes);

#endif