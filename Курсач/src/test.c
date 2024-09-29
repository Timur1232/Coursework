#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"
#include "file_work/file_work.h"
#include "file_work/parser/parser.h"

#ifdef _TEST

void procces_error(ParserErrors err)
{
	switch (err)
	{
	case ALL_GOOD:
		printf("all good\n");
		break;
	case FILE_OPEN_ERR:
		printf("file open erron\n");
		break;
	case BUFF_SIZE_EXCEEDED:
		printf("buffer size exceeded\n");
		break;
	case NO_OPEN_BRACKET:
		printf("expected open bracket\n");
		break;
	case SCAN_INT_ERR:
		printf("int scan error\n");
		break;
	case SCAN_FLOAT_ERR:
		printf("float scan error\n");
		break;
	case SCAN_STR_ERR:
		printf("string scan error\n");
		break;
	case MULTIPLE_VARS:
		printf("multiple variables declared\n");
		break;
	case EXPECT_ASSIGN:
		printf("expected assign token\n");
		break;
	case EXPECT_VALUE:
		printf("expected value token\n");
		break;
	}
}

int main()
{
	FILE* file = fopen("input/syntax.txt", "r");
	FECNote note = init_note(),
		note2 = init_note();

	ParserErrors err = scan_note(file, &note);

	procces_error(err);
	err = scan_note(file, &note2);
	procces_error(err);

	print_note(&note);
	print_note(&note2);


	return 0;
}

#endif