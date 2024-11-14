#ifndef FILE_WORK_H
#define FILE_WORK_H

#include <stdio.h>

#include "../list/list.h"
#include "parser/tokenizer.h"

// Загрузка списка из файла
ErrorHandler scan_note_list(const char* fileName, ListPtr fecNotes);
int scan_bin_note_list(const char* fileName, ListPtr fecNotes);

// Сохранение списка в файл
int save_note_list(const char* fileName, ListPtr fecNotes);
int save_bin_note_list(const char* fileName, ListPtr fecNotes);

// Сохранение результатов вычисления в файл
int save_results(const char* fileName, ListPtr fecNotes);

#endif // FILE_WORK_H