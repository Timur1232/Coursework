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

// Проверка на повторения полей
int check_repeat(RepeatObserver* observer, Token token);	

// Обработка ошибок
char* proccess_error(char* buff, ErrorHandler error);
char* print_token(char* buff, TokenVar* token);

#endif // TOKENIZER_H