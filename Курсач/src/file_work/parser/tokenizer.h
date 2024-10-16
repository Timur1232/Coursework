#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <stdio.h>

#include <types.h>
#include "../../fec_note/fec_note.h"

#define MAX_BUFFER_SIZE 513

// Тип токена
typedef enum Token
{
    // Переменные (поля структуры)
    SERIAL_NUM_VAR = 0,
    FACTORY_NUM_VAR = 1,
    DIR_NAME_VAR = 2,
    ENG_NAME_VAR = 3,
    CONS_PLAN_VAR = 4,
    CONS_REAL_VAR = 5,

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

    END,

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
    EXPECT_VAR,
    EXPECT_ASSIGN,
    EXPECT_VALUE,
    EXPECT_INT,
    EXPECT_FLOAT,
    EXPECT_STR,

    UNEXPECTED_ERROR,
    ALLOCATION_FAILURE
} TokenizerErrors;

// Токен
typedef struct TokenVar
{
    Token type;
    union
    {
        uint_32 intValue;
        float floatValue;
        char stringValue[MAX_STRING_SIZE];
    } value;
    int_32 line;
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
    byte serialNumber : 1;
    byte factoryNumber : 1;
    byte directorFullName : 1;
    byte engineerFullName : 1;
    byte energyConsPlan : 1;
    byte energyConsReal : 1;
} RepeatObserver;

// Обработчик ошибок
typedef struct ErrorHandler
{
    int_32 line;
    TokenizerErrors err;
} ErrorHandler;

// Инициализаторы
TokenQueue init_token_queue();
TokenVar init_token();
RepeatObserver init_observer();
ErrorHandler init_error_handler();

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
void shift_buff(char* buff, uint_64 n);
void ignore_white_space(char* buff);
char* read_line(char* buff, FILE* file);

// Вспомогательные логические функции
int eob(char* buff);										// Проверка на конец буфера
int white_space(char ch);									// Проверка символа на ' ', '\t' или '\n'
int divider(char ch);										// Проверка символа на ';' и ','
int is_digit(char ch);										// Проверка если символ - цифра
int contain_period(char* str);								// Проверка на наличие в строке '.'
int my_strcmp(const char* str1, const char* str2);			// Собственная функция strcmp
int check_repeat(RepeatObserver* observer, Token token);	// Проверка на повторения полей

// Считывание значений
int get_int(char* buff, uint_32* data);						// Считывание int значения из буфера
int get_float(char* buff, float* data);						// Считывание float значения из буфера
int get_str(char* buff, char* data);						// Считывание char* значения из буфера
int get_value(char* buff, TokenVar* token);					// Общее считывание значения из буфера

// Обработка ошибок
char* proccess_error(char* buff, ErrorHandler error);
char* print_token(char* buff, TokenVar* token);

#endif