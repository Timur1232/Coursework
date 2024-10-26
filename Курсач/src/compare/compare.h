#ifndef COMPARE_H
#define COMPARE_H

#include <types.h>
#include "..\list\list.h"

// Массив функций сравнения
extern int (* const COMPARE_FUNC_ARRAY[6]) (ConstValuePtr val1, ConstValuePtr val2);

typedef enum Compare
{
    SER_NUM = 0,
    FAC_NUM = 1,
    DIR_NAME = 2,
    ENG_NAME = 3,
    CONS_PLAN = 4,
    CONS_REAL = 5
} Compare;

int ser_num(ConstValuePtr val1, ConstValuePtr val2);
int fac_num(ConstValuePtr val1, ConstValuePtr val2);
int dir_name(ConstValuePtr val1, ConstValuePtr val2);
int eng_name(ConstValuePtr val1, ConstValuePtr val2);
int cons_plan(ConstValuePtr val1, ConstValuePtr val2);
int cons_real(ConstValuePtr val1, ConstValuePtr val2);

int my_strcmp(const char* str1, const char* str2);

#endif // COMPARE_H