#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include "../../fec_note/fec_note.h"

#define BUFFER_SIZE 512

// ��� ������
typedef enum Token
{
	// ���������� (���� ���������)
	SERIAL_NUM = 0,
	FACTORY_NUM = 1,
	DIR_NAME = 2,
	ENG_NAME = 3,
	CONS_PLAN = 4,
	CONS_REAL = 5,

	// ����������� �������
	OPEN_BRACKET = '{',
	CLOSE_BRAKET = '}',
	EQUAL_SIGN = '=',
	QUOTE_MARK = '\"',
	COMMENT = '#',
	COMMA = ',',
	SEMICOLON = ';',
	EMPTY_LINE,

	// ���� ������
	INT_TYPE = 6,
	FLOAT_TYPE = 7,
	STRING_TYPE = 8,

	NONE = -1
} Token;

// ��� ���� ������
typedef enum TokenType
{
	VAR,
	ASSIGN,
	SCOPE,
	VALUE_TYPE,
	SPEC,
	DIVIDER,

	NONE_TYPE = -1
} TokenType;

// ��������� ������ �����������
typedef enum TokenizerErrors
{
	// ��� ������
	ALL_GOOD = 0,

	// ������������ �����
	UNRECOGNOZABLE_TOKEN,

	// ������ � �������
	FILE_OPEN_ERR,
	FILE_ERROR,

	// ������������ ������
	BUFF_SIZE_EXCEEDED,

	// ������ � ������������ ������
	NO_OPEN_BRACKET,
	NO_CLOSE_BRACKET,

	// ������ ����� ������
	SCAN_INT_ERR,
	SCAN_FLOAT_ERR,
	SCAN_STR_ERR,

	// ������������ ������ � ���� � �� �� ����������
	MULTIPLE_VARS,

	// ��������� ������
	EXPECT_ASSIGN,
	EXPECT_VALUE,
	EXPECT_INT,
	EXPECT_FLOAT,
	EXPECT_STR,

	UNEXPECTED_ERROR
} TokenizerErrors;

// �����
typedef struct TokenVar
{
	Token type;
	union
	{
		int intValue;
		float floatValue;
		char stringValue[MAX_STRING_SIZE];
	} value;
} TokenVar;

// ������ ���� �������
typedef struct TokenNode
{
	TokenVar token;
	struct TokenNode* next;
} TokenNode;

typedef struct TokenQueue
{
	int size;
	TokenNode* beg;
	TokenNode* end;
} TokenQueue;

// ����������� �� ���������
typedef struct RepeatObserver
{
	char serialNumber : 1;
	char factoryNumber : 1;
	char directorFullName : 1;
	char engineerFullName : 1;
	char energyConsPlan : 1;
	char energyConsReal : 1;
} RepeatObserver;

// ���������� ������
typedef struct ErrorHandler
{
	int line;
	TokenizerErrors err;
} ErrorHandler;

// ��������������
TokenQueue init_token_queue();
TokenVar init_token();
RepeatObserver init_observer();

// ������� ���������� � ��������� ������� �� ���������� �����
ErrorHandler tokenize(TokenQueue* tokens, FILE* file);

// ������ (�������)
void clear_tokens(TokenQueue* tokens);
void push_token(TokenQueue* tokens, TokenVar* token);
void pop_token(TokenQueue* tokens);
TokenVar* get_token(TokenQueue* tokens);
int empty_tokens(TokenQueue* tokens);

// ������
Token scan_token(char* buff);
TokenType token_type(Token token);
Token var_type(Token token);

// �������������� ������
void shift_buff(char* buff, int n);
void ignore_white_space(char* buff);

// ��������������� ���������� �������
int exceded_buff(char* buff);								// �������� �� ������������ ������
int eob(char* buff);										// �������� �� ����� ������
int white_space(char ch);									// �������� ������� �� ' ', '\t' ��� '\n'
int divider(char ch);										// �������� ������� �� ';' � ','
int is_digit(char ch);										// �������� ���� ������ - �����
int contain_period(char* str);								// �������� �� ������� � ������ '.'
int my_strcmp(const char* str1, const char* str2);			// ����������� ������� strcmp
int check_repeat(RepeatObserver* observer, Token token);	// �������� �� ���������� �����

// ���������� ��������
int get_int(char* buff, int* data);							// ���������� int �������� �� ������
int get_float(char* buff, float* data);						// ���������� float �������� �� ������
int get_str(char* buff, char* data);						// ���������� char* �������� �� ������
int get_value(char* buff, TokenVar* token);					// ����� ���������� �������� �� ������

// ��������� ������
char* proccess_error(char* buff, ErrorHandler error);
char* print_token(char* buff, TokenVar* token);

#endif