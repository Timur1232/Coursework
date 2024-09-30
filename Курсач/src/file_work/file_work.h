#ifndef FILE_WORK_H
#define FILE_WORK_H

#include "../fec_note/fec_note.h"
#include "../list/list.h"
#include "parser/tokenizer.h"

#include <stdio.h>

// Загрузка из файла
ErrorHandler scan_note_list(const char* fileName, ListPtr fecNotes);
int scan_bin_note_list(const char* fileName, ListPtr fecNotes);

// Сохранение в файл
int save_note_list(const char* fileName, ListPtr fecNotes);
int save_bin_note_list(const char* fileName, ListPtr fecNotes);

#endif