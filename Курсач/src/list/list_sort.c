#include "list.h"
#include "list_sort.h"

#include <string.h>

void sort(ListPtr list, int (*compare)(ConstValuePtr val1, ConstValuePtr val2))
{
	if (!list) { return; }
	for (FOR_RANGE(i, *list))
	{
		for (Iterator j = list->end; j != i; DECREMENT(j))
		{
			if (compare(&j->data, &j->prev->data) < 0)
			{
				SWAP(j->prev, j);
			}
		}
	}
}

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