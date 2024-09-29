#ifndef PARSER_H
#define PARSER_H

#define BUFFER_SIZE 512

#include "../../fec_note/fec_note.h"

// Токены
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

// Типы токенов
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

// Возможные ошибки парсера
typedef enum ParserErrors
{
	// Нет ошибки
	ALL_GOOD = 0,
	FILE_ENDS,

	// Неправильный токен
	UNRECOGNOZABLE_TOKEN,

	// Ошибка в открытии файла
	FILE_OPEN_ERR,

	// Переполнение буфера
	BUFF_SIZE_EXCEEDED,

	// Ошибки в расставлении границ
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
	EXPECT_STR
} ParserErrors;

// Наблюдатель за повторами переменных
typedef struct InputObserver
{
	char serialNumber : 1;
	char factoryNumber : 1;
	char directorFullName : 1;
	char engineerFullName : 1;
	char energyConsPlan : 1;
	char energyConsReal : 1;
} InputObserver;

// Все необходимые данные для парсинга
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

// Обработчик ошибок
typedef struct ParserErrorHandler
{
	int line;
	ParserErrors err;
} ParserErrorHandler;

// Инициализация
ParserHandler init_parser();
InputObserver init_input();

// Редактирование буфера
void shift_buff(char* buff, int n);
void ignore_white_space(char* buff);

// Обработка токенов
Token get_token(char* buff);
TokenType token_type(Token token);

// Вспомогательные логические функции
int eob(char* buff);
int white_space(char ch);
int divider(char ch);
int is_digit(char ch);
int contain_period(char* str);
int my_strcmp(const char* str1, const char* str2);

#endif