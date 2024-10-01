#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"
#include "file_work/file_work.h"
#include "file_work/parser/tokenizer.h"

#ifdef _TEST

#if 1
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
#else
int main()
{
	system("chcp 65001");

	FILE* file = fopen("input/example.txt", "r");
	TokenQueue tokens = init_token_queue();
	char buff[100] = { 0 };

	puts(proccess_error(buff, tokenize(&tokens, file)));
	fclose(file);

	while (!empty_tokens(&tokens))
	{
		puts(print_token(buff, get_token(&tokens)));
		pop_token(&tokens);
	}

	return 0;
}
#endif

#endif