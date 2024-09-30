#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"
#include "file_work/file_work.h"
#include "file_work/parser/tokenizer.h"

#ifdef _TEST

int main()
{
	system("chcp 65001");

	List list = init_list();
	char buff[100] = { 0 };

	puts(proccess_error(buff, scan_note_list("input/example.txt", &list)));

	print_list(&list);

	save_note_list("output/test.txt", &list);

	clear(&list);

	return 0;
}

#endif