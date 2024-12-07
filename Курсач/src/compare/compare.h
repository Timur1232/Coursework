#ifndef COMPARE_H
#define COMPARE_H

#include <types.h>
#include "../fec_note/fec_note.h"

// Массив функций сравнения
extern int (* const COMPARE_FUNC_ARRAY[6]) (const FECNote* val1, const FECNote* val2);

typedef enum Compare
{
    SER_NUM = 0,
    FAC_NUM = 1,
    DIR_NAME = 2,
    ENG_NAME = 3,
    CONS_PLAN = 4,
    CONS_REAL = 5
} Compare;

// Функции сравнения структур по их полям
int ser_num(const FECNote* val1, const FECNote* val2);
int fac_num(const FECNote* val1, const FECNote* val2);
int dir_name(const FECNote* val1, const FECNote* val2);
int eng_name(const FECNote* val1, const FECNote* val2);
int cons_plan(const FECNote* val1, const FECNote* val2);
int cons_real(const FECNote* val1, const FECNote* val2);

int my_strcmp(const char* str1, const char* str2);

#endif // COMPARE_H