#ifdef _TEST

#include <stdio.h>
#include <string.h>
#include <time.h>

#include "list/list.h"
#include "fec_note/fec_note.h"
#include "file_work/file_work.h"
#include "file_work/parser/tokenizer.h"

const char* const names[] = {
	"Timur Bai",
	"Kirill Kv",
	"Danil Bl",
	"Dmitry K",
	"Boris L",
	"Maks L",
	"Egor Letov",
	"Serj Tankian",
	"Daron Malakian",
	"Chester B",
	"Tom York",
	"Kurt Cobain",
	"Yuri Gagarin",
};

FECNote gen_random_note()
{
	static int count = 1;
	FECNote note;

	note.serialNumber = count++;
	note.factoryNumber = rand() % 100;
	strcpy(note.directorFullName, names[rand() % 13]);
	strcpy(note.engineerFullName, names[rand() % 13]);
	note.energyConsPlan = (float)(rand() % 200 + 50) + (float)(rand() % 100) * 0.01f;
	note.energyConsReal = (float)(rand() % 200 + 50) + (float)(rand() % 100) * 0.01f;

	return note;
}

#if 1
int main()
{
	system("chcp 65001");
	srand(time(NULL));


	List list = init_list();
	char buff[100] = { 0 };

	/*for (int i = 0; i < 100; i++)
	{
		FECNote note = gen_random_note();
		push_back(&list, &note);
	}*/

	puts(proccess_error(buff, scan_note_list("input/random_gen.txt", &list)));

	print_list(&list);

	//save_note_list("output/random_gen.txt", &list);

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