#include "fec_note.h"

FECNote init_note()
{
    FECNote note = {
        0, 0, "", "", 0.0f, 0.0f
    };
    return note;
}

int (* const COMPARE_FUNC_ARRAY[6]) (const FECNote* val1, const FECNote* val2) = {
    ser_num, fac_num, dir_name, eng_name, cons_plan, cons_real
};

int ser_num(const FECNote* val1, const FECNote* val2)
{
    if (val1->serialNumber < val2->serialNumber) return -1;
    if (val1->serialNumber == val2->serialNumber) return 0;
    return 1;
}

int fac_num(const FECNote* val1, const FECNote* val2)
{
    if (val1->factoryNumber < val2->factoryNumber) return -1;
    if (val1->factoryNumber == val2->factoryNumber) return 0;
    return 1;
}

int dir_name(const FECNote* val1, const FECNote* val2)
{
    return my_strcmp(val1->directorFullName, val2->directorFullName);
}

int eng_name(const FECNote* val1, const FECNote* val2)
{
    return my_strcmp(val1->engineerFullName, val2->engineerFullName);
}

int cons_plan(const FECNote* val1, const FECNote* val2)
{
    if (val1->energyConsPlan < val2->energyConsPlan) return -1;
    if (val1->energyConsPlan > val2->energyConsPlan) return 1;
    return 0;
}

int cons_real(const FECNote* val1, const FECNote* val2)
{
    if (val1->energyConsReal < val2->energyConsReal) return -1;
    if (val1->energyConsReal > val2->energyConsReal) return 1;
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