#ifndef LIST_SORT_H
#define LIST_SORT_H

#include "list.h"

void sort(ListPtr list, int (*compare)(ConstValuePtr val1, ConstValuePtr val2));

int ser_num(ConstValuePtr val1, ConstValuePtr val2);
int fac_num(ConstValuePtr val1, ConstValuePtr val2);
int dir_name(ConstValuePtr val1, ConstValuePtr val2);
int eng_name(ConstValuePtr val1, ConstValuePtr val2);
int cons_plan(ConstValuePtr val1, ConstValuePtr val2);
int cons_real(ConstValuePtr val1, ConstValuePtr val2);

#endif // LIST_SORT_H