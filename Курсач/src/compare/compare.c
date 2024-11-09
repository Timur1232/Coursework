#include "compare.h"

#include <string.h>

#include "..\list\list.h"

int (* const COMPARE_FUNC_ARRAY_ASC[6]) (ConstValuePtr val1, ConstValuePtr val2) = {
    ser_num_asc, fac_num_asc, dir_name_asc, eng_name_asc, cons_plan_asc, cons_real_asc
};

int (* const COMPARE_FUNC_ARRAY_DESC[6]) (ConstValuePtr val1, ConstValuePtr val2) = {
    ser_num_desc, fac_num_desc, dir_name_desc, eng_name_desc, cons_plan_desc, cons_real_desc
};

int ser_num_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->serialNumber < val2->serialNumber) return -1;
    if (val1->serialNumber == val2->serialNumber) return 0;
    return 1;
}

int fac_num_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->factoryNumber < val2->factoryNumber) return -1;
    if (val1->factoryNumber == val2->factoryNumber) return 0;
    return 1;
}

int dir_name_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    return strcmp(val1->directorFullName, val2->directorFullName);
}

int eng_name_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    return strcmp(val1->engineerFullName, val2->engineerFullName);
}

int cons_plan_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->energyConsPlan < val2->energyConsPlan) return -1;
    if (val1->energyConsPlan > val2->energyConsPlan) return 1;
    return 0;
}

int cons_real_asc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->energyConsReal < val2->energyConsReal) return -1;
    if (val1->energyConsReal > val2->energyConsReal) return 1;
    return 0;
}

int ser_num_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->serialNumber < val2->serialNumber) return 1;
    if (val1->serialNumber == val2->serialNumber) return 0;
    return -1;
}

int fac_num_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->factoryNumber < val2->factoryNumber) return 1;
    if (val1->factoryNumber == val2->factoryNumber) return 0;
    return -1;
}

int dir_name_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    return -strcmp(val1->directorFullName, val2->directorFullName);
}

int eng_name_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    return -strcmp(val1->engineerFullName, val2->engineerFullName);
}

int cons_plan_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->energyConsPlan < val2->energyConsPlan) return 1;
    if (val1->energyConsPlan > val2->energyConsPlan) return -1;
    return 0;
}

int cons_real_desc(ConstValuePtr val1, ConstValuePtr val2)
{
    if (val1->energyConsReal < val2->energyConsReal) return 1;
    if (val1->energyConsReal > val2->energyConsReal) return -1;
    return 0;
}

int my_strcmp(const char* str1, const char* str2)
{
    while (*str1 == *str2)
    {
        str1++;
        str2++;
    }
    if (*str1 == '\0') return 0;
    return *str1 - *str2;
}