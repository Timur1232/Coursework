#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"

#ifdef _TEST

int main()
{
	FILE* file = fopen("input/test.txt", "r");
	char str[5];

	fgets(str, 5, file);
	puts(str);
	fgets(str, 5, file);
	puts(str);

	fclose(file);

	return 0;
}

#endif