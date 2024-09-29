#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 128

#include "../../fec_note/fec_note.h"

typedef enum Token
{
	SERIAL_NUM = 0,
	FACTORY_NUM = 1,
	DIR_NAME = 2,
	ENG_NAME = 3,
	CONS_PLAN = 4,
	CONS_REAL = 5,

	OPEN_BRACKET = '{',
	CLOSE_BRAKET = '}',
	EQUAL_SIGN = '=',
	QUOTE_MARK = '\"',
	COMMENT = '#',
	EMPTY_LINE,

	INT_TYPE = 6,
	FLOAT_TYPE = 7,
	STRING_TYPE = 8,

	NONE = -1
} Token;

typedef enum TokenType
{
	VAR,
	ASSIGN,
	SCOPE,
	VALUE_TYPE,
	SPEC,

	NONE_TYPE = -1
} TokenType;

typedef enum ParserErrors
{
	ALL_GOOD = 0,
	FILE_ENDS,

	FILE_OPEN_ERR,

	BUFF_SIZE_EXCEEDED,

	NO_OPEN_BRACKET,
	NO_CLOSE_BRACKET,

	SCAN_INT_ERR,
	SCAN_FLOAT_ERR,
	SCAN_STR_ERR,

	MULTIPLE_VARS,

	EXPECT_ASSIGN,
	EXPECT_VALUE,
	EXPECT_INT,
	EXPECT_FLOAT,
	EXPECT_STR
} ParserErrors;

typedef struct InputObserver
{
	char serialNumber : 1;
	char factoryNumber : 1;
	char directorFullName : 1;
	char engineerFullName : 1;
	char energyConsPlan : 1;
	char energyConsReal : 1;
} InputObserver;

typedef struct ParserHandler
{
	int shouldClose;
	int buffSizeExceeded;

	char buff[BUFFER_SIZE];
	Token token;

	InputObserver input;
	union
	{
		int intValue;
		float floatValue;
		char stringValue[MAX_STRING_SIZE];
	} scanValue;
} ParserHandler;

ParserHandler init_parser();
InputObserver init_input();

void shift_buff(char* buff, int n);
void ignore_white_space(char* buff);

Token get_token(char* buff);
TokenType token_type(Token token);

int eob(char* buff);
int white_space(char ch);
int is_digit(char ch);
int contain(char* str, char ch);
int my_strcmp(const char* str1, const char* str2);

#endif