#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"

#ifdef _TEST

int main()
{
	FECNote note = init_note();

	scan_note("input/in.txt", &note, 1);
	print_note(&note);

	return 0;
}

#endif