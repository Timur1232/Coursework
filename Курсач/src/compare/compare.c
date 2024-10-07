#include "compare.h"
#include "..\list\list.h"

#include <string.h>

int ser_num(ConstValuePtr val1, ConstValuePtr val2)
{
	if (val1->serialNumber < val2->serialNumber) return -1;
	if (val1->serialNumber == val2->serialNumber) return 0;
	return 1;
}

int fac_num(ConstValuePtr val1, ConstValuePtr val2)
{
	if (val1->factoryNumber < val2->factoryNumber) return -1;
	if (val1->factoryNumber == val2->factoryNumber) return 0;
	return 1;
}

int dir_name(ConstValuePtr val1, ConstValuePtr val2)
{
	return strcmp(val1->directorFullName, val2->directorFullName);
}

int eng_name(ConstValuePtr val1, ConstValuePtr val2)
{
	return strcmp(val1->engineerFullName, val2->engineerFullName);
}

int cons_plan(ConstValuePtr val1, ConstValuePtr val2)
{
	if (val1->energyConsPlan < val2->energyConsPlan) return -1;
	if (val1->energyConsPlan > val2->energyConsPlan) return 1;
	return 0;
}

int cons_real(ConstValuePtr val1, ConstValuePtr val2)
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