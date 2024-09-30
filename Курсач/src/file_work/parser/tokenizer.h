#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>
#include "../../fec_note/fec_note.h"

#define BUFFER_SIZE 512

// Тип токена
typedef enum Token
{
	// Переменные (поля структуры)
	SERIAL_NUM = 0,
	FACTORY_NUM = 1,
	DIR_NAME = 2,
	ENG_NAME = 3,
	CONS_PLAN = 4,
	CONS_REAL = 5,

	// Специальные символы
	OPEN_BRACKET = '{',
	CLOSE_BRAKET = '}',
	EQUAL_SIGN = '=',
	QUOTE_MARK = '\"',
	COMMENT = '#',
	COMMA = ',',
	SEMICOLON = ';',
	EMPTY_LINE,

	// Типы данных
	INT_TYPE = 6,
	FLOAT_TYPE = 7,
	STRING_TYPE = 8,

	NONE = -1
} Token;

// Тип типа токена
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

// Возможные ошибки токенизации
typedef enum TokenizerErrors
{
	// Нет ошибки
	ALL_GOOD = 0,

	// Неправильный токен
	UNRECOGNOZABLE_TOKEN,

	// Ошибки с файлами
	FILE_OPEN_ERR,
	FILE_ERROR,

	// Переполнение буфера
	BUFF_SIZE_EXCEEDED,

	// Ошибки в расставлении скобок
	NO_OPEN_BRACKET,
	NO_CLOSE_BRACKET,

	// Ошибки типов данных
	SCAN_INT_ERR,
	SCAN_FLOAT_ERR,
	SCAN_STR_ERR,

	// Многоразовая запись в одну и ту же переменную
	MULTIPLE_VARS,

	// Ожидаемые токены
	EXPECT_ASSIGN,
	EXPECT_VALUE,
	EXPECT_INT,
	EXPECT_FLOAT,
	EXPECT_STR,

	UNEXPECTED_ERROR
} TokenizerErrors;

// Токен
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

// Список типа очередь
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

// Наблюдатель за повторами
typedef struct RepeatObserver
{
	char serialNumber : 1;
	char factoryNumber : 1;
	char directorFullName : 1;
	char engineerFullName : 1;
	char energyConsPlan : 1;
	char energyConsReal : 1;
} RepeatObserver;

// Обработчик ошибок
typedef struct ErrorHandler
{
	int line;
	TokenizerErrors err;
} ErrorHandler;

// Инициализаторы
TokenQueue init_token_queue();
TokenVar init_token();
RepeatObserver init_observer();

// Функция считывания и обработки токенов из текстового файла
ErrorHandler tokenize(TokenQueue* tokens, FILE* file);

// Список (очередь)
void clear_tokens(TokenQueue* tokens);
void push_token(TokenQueue* tokens, TokenVar* token);
void pop_token(TokenQueue* tokens);
TokenVar* get_token(TokenQueue* tokens);
int empty_tokens(TokenQueue* tokens);

// Токены
Token scan_token(char* buff);
TokenType token_type(Token token);
Token var_type(Token token);

// Редактирование буфера
void shift_buff(char* buff, int n);
void ignore_white_space(char* buff);

// Вспомогательные логические функции
int exceded_buff(char* buff);								// Проверка на переполнение буфера
int eob(char* buff);										// Проверка на конец буфера
int white_space(char ch);									// Проверка символа на ' ', '\t' или '\n'
int divider(char ch);										// Проверка символа на ';' и ','
int is_digit(char ch);										// Проверка если символ - цифра
int contain_period(char* str);								// Проверка на наличие в строке '.'
int my_strcmp(const char* str1, const char* str2);			// Собственная функция strcmp
int check_repeat(RepeatObserver* observer, Token token);	// Проверка на повторения полей

// Считывание значений
int get_int(char* buff, int* data);							// Считывание int значения из буфера
int get_float(char* buff, float* data);						// Считывание float значения из буфера
int get_str(char* buff, char* data);						// Считывание char* значения из буфера
int get_value(char* buff, TokenVar* token);					// Общее считывание значения из буфера

// Обработка ошибок
char* proccess_error(char* buff, ErrorHandler error);
char* print_token(char* buff, TokenVar* token);

#endif