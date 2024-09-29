#include <stdio.h>

#include "list/list.h"
#include "fec_note/fec_note.h"
#include "file_work/file_work.h"
#include "file_work/parser/parser.h"

#ifdef _TEST

void procces_error(ParserErrorHandler error)
{
	switch (error.err)
	{
	case ALL_GOOD:
		printf("all good\n");
		break;
	case FILE_OPEN_ERR:
		printf("file open erron\n");
		break;
	case BUFF_SIZE_EXCEEDED:
		printf("buffer size exceeded; line: %d\n", error.line);
		break;
	case NO_OPEN_BRACKET:
		printf("expected open bracket; line: %d\n", error.line);
		break;
	case NO_CLOSE_BRACKET:
		printf("expected close bracket; line: %d\n", error.line);
		break;
	case SCAN_INT_ERR:
		printf("int scan error; line: %d\n", error.line);
		break;
	case SCAN_FLOAT_ERR:
		printf("float scan error; line: %d\n", error.line);
		break;
	case SCAN_STR_ERR:
		printf("string scan error; line: %d\n", error.line);
		break;
	case MULTIPLE_VARS:
		printf("multiple variables declared; line: %d\n", error.line);
		break;
	case EXPECT_ASSIGN:
		printf("expected assign token; line: %d\n", error.line);
		break;
	case EXPECT_VALUE:
		printf("expected value token; line: %d\n", error.line);
		break;
	case EXPECT_INT:
		printf("expected int type; line: %d\n", error.line);
		break;
	case EXPECT_FLOAT:
		printf("expected float type; line: %d\n", error.line);
		break;
	case EXPECT_STR:
		printf("expected string type; line: %d\n", error.line);
		break;
	case UNRECOGNOZABLE_TOKEN:
		printf("unrecognizable token; line: %d\n", error.line);
		break;
	default:
		printf("Unexpected code: %d; line: %d\n", error.err, error.line);
	}
}

int main()
{
	system("chcp 65001");

	List list = init_list();

	puts("scan list input/example.txt");
	procces_error(scan_note_list("input/example.txt", &list));

	print_list(&list);

	return 0;
}

#endif