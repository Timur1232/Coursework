#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <macros.h>
#include <types.h>
#include "../../compare/compare.h"
#include "../../log/log.h"

static const char* const VarTokens[] = {
    "serialNumber",
    "factoryNumber",
    "directorName",
    "engineerName",
    "energyConsPlan",
    "energyConsReal"
};

/*=====================================[Функции]=====================================*/

/*=====================================[Инициализаторы]=====================================*/

TokenQueue init_token_queue()
{
    TokenQueue tokens = { 0, NULL, NULL };
    return tokens;
}

TokenVar init_token()
{
    TokenVar token = { NONE, {0} };
    return token;
}

RepeatObserver init_observer()
{
    RepeatObserver observer = { 0, 0, 0, 0, 0, 0 };
    return observer;
}

ErrorHandler init_error_handler()
{
    ErrorHandler error = { 0, ALL_GOOD };
    return error;
}

/*=====================================[Токенайзер]=====================================*/

ErrorHandler tokenize(TokenQueue* tokens, FILE* file)
{
    clear_tokens(tokens);

    ErrorHandler error = init_error_handler();
    char* buff = (char*)malloc(MAX_BUFFER_SIZE);
    if (!buff)
    {
        error.err = ALLOCATION_FAILURE;
        return error;
    }
    memset(buff, 0, MAX_BUFFER_SIZE);
    Bool openBracket = 0;
    TokenVar token = init_token();

    while ((buff = read_line(buff, file)) != NULL)
    {
        if (ferror(file))
        {
            error.err = FILE_ERROR;
            free(buff);
            return error;
        }

        error.line++;

        while (!eob(buff))
        {
            token = init_token();
            token.type = scan_token(buff);

            if (token_type(token.type) == SPEC)
            {
                break;
            }
            else if (token_type(token.type) == DIVIDER)
            {
                continue;
            }
            else if (token.type == NONE)
            {
                error.err = UNRECOGNOZABLE_TOKEN;
                free(buff);
                return error;
            }
            else if (token_type(token.type) == VALUE_TYPE || token.type == QUOTE_MARK)
            {
                if (token.type == QUOTE_MARK)
                {
                    token.type = STRING_TYPE;
                }

                error.err = get_value(buff, &token);
                if (error.err != ALL_GOOD)
                {
                    free(buff);
                    return error;
                }
            }

            token.line = error.line;
            push_token(tokens, &token);
        }
    }

    token.type = END;
    token.line = error.line;
    push_token(tokens, &token);

    free(buff);

    return error;
}

/*=====================================[Список]=====================================*/

void clear_tokens(TokenQueue* tokens)
{
    if (!tokens->size)
    {
        return;
    }

    TokenNode *node1 = tokens->beg,
              *node2 = tokens->beg->next;

    while (node2)
    {
        free(node1);
        node1 = node2;
        INCREMENT(node2);
    }
    free(node1);
    tokens->size = 0;
}

void push_token(TokenQueue* tokens, TokenVar* token)
{
    TokenNode* newNode = NEW(TokenNode, 1);
    if (!newNode)
    {
        LOG(ERR, "tokenozer.c", "push_token()", "malloc() returned NULL", LOG_FILE);
        exit(-1);
    }
    newNode->next = NULL;
    newNode->token = *token;

    if (!tokens->size)
    {
        tokens->beg = tokens->end = newNode;
    }
    else
    {
        tokens->end->next = newNode;
        INCREMENT(tokens->end);
    }
    tokens->size++;
}

void pop_token(TokenQueue* tokens)
{
    if (!tokens->size)
    {
        return;
    }
    
    if (tokens->size == 1)
    {
        clear_tokens(tokens);
    }
    else
    {
        TokenNode* delNode = tokens->beg;
        INCREMENT(tokens->beg);
        free(delNode);
        tokens->size--;
    }
}

TokenVar* get_token(TokenQueue* tokens)
{
    return &tokens->beg->token;
}

int empty_tokens(TokenQueue* tokens)
{
    return tokens->size == 0;
}

/*=====================================[Токены]=====================================*/

Token scan_token(char* buff)
{
    ignore_white_space(buff);

    uint_64 shift = 0;
    Token returnValue = NONE;

    if (buff[0] == OPEN_BRACKET)
    {
        shift = 1;
        returnValue = OPEN_BRACKET;
    }
    else if (buff[0] == CLOSE_BRAKET)
    {
        shift = 1;
        returnValue = CLOSE_BRAKET;
    }
    else if (buff[0] == EQUAL_SIGN)
    {
        shift = 1;
        returnValue = EQUAL_SIGN;
    }
    else if (buff[0] == QUOTE_MARK)
    {
        shift = 1;
        returnValue = QUOTE_MARK;
    }
    else if (buff[0] == COMMENT)
    {
        shift = 1;
        returnValue = COMMENT;
    }
    else if (buff[0] == COMMA)
    {
        shift = 1;
        returnValue = COMMA;
    }
    else if (buff[0] == SEMICOLON)
    {
        shift = 1;
        returnValue = SEMICOLON;
    }
    else if (buff[0] == '\0')
    {
        return EMPTY_LINE;
    }
    else if (is_digit(buff[0]))
    {
        if (contain_period(buff))
        {
            return FLOAT_TYPE;
        }
        return INT_TYPE;
    }
    else if (my_strcmp(VarTokens[SERIAL_NUM_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[SERIAL_NUM_VAR]);
        returnValue = SERIAL_NUM_VAR;
    }
    else if (my_strcmp(VarTokens[FACTORY_NUM_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[FACTORY_NUM_VAR]);
        returnValue = FACTORY_NUM_VAR;
    }
    else if (my_strcmp(VarTokens[DIR_NAME_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[DIR_NAME_VAR]);
        returnValue = DIR_NAME_VAR;
    }
    else if (my_strcmp(VarTokens[ENG_NAME_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[ENG_NAME_VAR]);
        returnValue = ENG_NAME_VAR;
    }
    else if (my_strcmp(VarTokens[CONS_PLAN_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[CONS_PLAN_VAR]);
        returnValue = CONS_PLAN_VAR;
    }
    else if (my_strcmp(VarTokens[CONS_REAL_VAR], buff) == 0)
    {
        shift = strlen(VarTokens[CONS_REAL_VAR]);
        returnValue = CONS_REAL_VAR;
    }
    else
    {
        return NONE;
    }

    shift_buff(buff, shift);
    return returnValue;
}

TokenType token_type(Token token)
{
    switch (token)
    {
    case SERIAL_NUM_VAR:
    case FACTORY_NUM_VAR:
    case DIR_NAME_VAR:
    case ENG_NAME_VAR:
    case CONS_PLAN_VAR:
    case CONS_REAL_VAR:
        return VAR;

    case EQUAL_SIGN:
        return ASSIGN;

    case OPEN_BRACKET:
    case CLOSE_BRAKET:
    case QUOTE_MARK:
        return SCOPE;

    case INT_TYPE:
    case FLOAT_TYPE:
    case STRING_TYPE:
        return VALUE_TYPE;

    case COMMENT:
    case EMPTY_LINE:
        return SPEC;

    case COMMA:
    case SEMICOLON:
        return DIVIDER;

    }
    return NONE_TYPE;
}

Token var_type(Token token)
{
    switch (token)
    {
    case SERIAL_NUM_VAR:
    case FACTORY_NUM_VAR:
        return INT_TYPE;
    case DIR_NAME_VAR:
    case ENG_NAME_VAR:
        return STRING_TYPE;
    case CONS_PLAN_VAR:
    case CONS_REAL_VAR:
        return FLOAT_TYPE;
    default:
        return NONE;
    }
}

/*=====================================[Буфер]=====================================*/

void shift_buff(char* buff, uint_64 n)
{
    if (!n) return;
    uint_64 size = 0;
    for (uint_64 i = n, j = 0; buff[i]; i++, j++)
    {
        buff[j] = buff[i];
        size++;
    }
    buff[size] = '\0';
}

void ignore_white_space(char* buff)
{
    uint_64 count = 0;
    for (uint_64 i = 0; buff[i] && white_space(buff[i]); i++)
    {
        count++;
    }
    shift_buff(buff, count);
}

char* read_line(char* buff, FILE* file)
{
    uint_64 capasity = MAX_BUFFER_SIZE;
    uint_64 i = 0;
    char ch = '\0';

    while (1)
    {
        ch = fgetc(file);

        if (ch == '\n' || ch == EOF)
        {
            break;
        }
        if (i == capasity - 1)
        {
            capasity += MAX_BUFFER_SIZE;
            char* reallocBuff = realloc(buff, capasity);
            if (!reallocBuff)
            {
                free(buff);
                return NULL;
            }
            buff = reallocBuff;
        }

        buff[i++] = ch;
    }
    
    buff[i] = '\0';
    return (ch == EOF) ? NULL : buff;
}

/*=====================================[Логические функции]=====================================*/

int eob(char* buff)
{
    return buff[0] == '\n' || buff[0] == '\0' || buff[0] == EOF;
}

int white_space(char ch)
{
    return ch == ' ' || ch == '\n' || ch == '\t';
}

int divider(char ch)
{
    return ch == ',' || ch == ';';
}

int is_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

int contain_period(char* str)
{
    while (*str != '\0' && !divider(*str) && !white_space(*str))
    {
        if (*(str++) == '.')
        {
            return 1;
        }
    }
    return 0;
}

int check_repeat(RepeatObserver* observer, Token token)
{
    switch (token)
    {
    case SERIAL_NUM_VAR:
    {
        if (observer->serialNumber)
            return 1;
        observer->serialNumber = 1;
        break;
    }
    case FACTORY_NUM_VAR:
    {
        if (observer->factoryNumber)
            return 1;
        observer->factoryNumber = 1;
        break;
    }
    case DIR_NAME_VAR:
    {
        if (observer->directorFullName)
            return 1;
        observer->directorFullName = 1;
        break;
    }
    case ENG_NAME_VAR:
    {
        if (observer->engineerFullName)
            return 1;
        observer->engineerFullName = 1;
        break;
    }
    case CONS_PLAN_VAR:
    {
        if (observer->energyConsPlan)
            return 1;
        observer->energyConsPlan = 1;
        break;
    }
    case CONS_REAL_VAR:
    {
        if (observer->energyConsReal)
            return 1;
        observer->energyConsReal = 1;
        break;
    }
    default:
        return -1;
    }
    return 0;
}

/*=====================================[Считывание значений]=====================================*/

int get_int(char* buff, uint_32* data)
{
    int count = 0;
    for (int i = 0; buff[i] && !white_space(buff[i]) && !divider(buff[i]); i++, count++);

    if (sscanf(buff, "%d", data) == 1)
        return count;

    return 0;
}

int get_float(char* buff, float* data)
{
    int count = 0;
    for (int i = 0; buff[i] && !white_space(buff[i]) && !divider(buff[i]); i++, count++);

    if (sscanf(buff, "%f", data) == 1)
        return count;
    return 0;
}

int get_str(char* buff, char* data)
{
    int i = 0;
    for (; buff[i] != '\"' && i < MAX_STRING_SIZE; i++)
    {
        data[i] = buff[i];
    }
    if (i >= MAX_STRING_SIZE && buff[i - 1] != '\"')
    {
        data[MAX_STRING_SIZE - 1] = '\0';
        while (buff[i] != '\"' && !white_space(buff[i])) i++;
        if (white_space(buff[i]))
        {
            return 0;
        }
    }
    else
    {
        data[i] = '\0';
    }
    return i + 1;
}

TokenizerErrors get_value(char* buff, TokenVar* token)
{
    uint_64 count = 0;
    ignore_white_space(buff);
    // Считывание значения, соответствующего типу токена
    switch (token->type)
    {
    case INT_TYPE:
        count = get_int(buff, &token->value.intValue);
        break;
    case FLOAT_TYPE:
        count = get_float(buff, &token->value.floatValue);
        break;
    case STRING_TYPE:
        count = get_str(buff, token->value.stringValue);
        break;
    default:
        return EXPECT_VALUE;
    }
    if (!count)
    {
        switch (token->type)
        {
        case INT_TYPE:
            return SCAN_INT_ERR;
        case FLOAT_TYPE:
            return SCAN_FLOAT_ERR;
        case STRING_TYPE:
            return SCAN_STR_ERR;
        }
        return UNEXPECTED_ERROR;
    }

    shift_buff(buff, count);
    return ALL_GOOD;
}

/*=====================================[Обработка ошибок]=====================================*/

char* proccess_error(char* buff, ErrorHandler error)
{
    switch (error.err)
    {
    case ALL_GOOD:
        sprintf(buff, "All good");
        break;
    case FILE_OPEN_ERR:
        sprintf(buff, "File open error");
        break;
    case FILE_ERROR:
        sprintf(buff, "File error");
        break;
    case NO_OPEN_BRACKET:
        sprintf(buff, "Expected open bracket; line: %d", error.line);
        break;
    case NO_CLOSE_BRACKET:
        sprintf(buff, "Expected close bracket; line: %d", error.line);
        break;
    case SCAN_INT_ERR:
        sprintf(buff, "Int scan error; line: %d", error.line);
        break;
    case SCAN_FLOAT_ERR:
        sprintf(buff, "Float scan error; line: %d", error.line);
        break;
    case SCAN_STR_ERR:
        sprintf(buff, "String scan error; line: %d", error.line);
        break;
    case MULTIPLE_VARS:
        sprintf(buff, "Multiple variables declared; line: %d", error.line);
        break;
    case EXPECT_VAR:
        sprintf(buff, "Expected variable token; line: %d", error.line);
        break;
    case EXPECT_ASSIGN:
        sprintf(buff, "Expected assign token; line: %d", error.line);
        break;
    case EXPECT_VALUE:
        sprintf(buff, "Expected value token; line: %d", error.line);
        break;
    case EXPECT_INT:
        sprintf(buff, "Expected int type; line: %d", error.line);
        break;
    case EXPECT_FLOAT:
        sprintf(buff, "Expected float type; line: %d", error.line);
        break;
    case EXPECT_STR:
        sprintf(buff, "Expected string type; line: %d", error.line);
        break;
    case UNRECOGNOZABLE_TOKEN:
        sprintf(buff, "Unrecognizable token; line: %d", error.line);
        break;
    default:
        sprintf(buff, "Unexpected error code: %d; line: %d", error.err, error.line);
    }
    return buff;
}

char* print_token(char* buff, TokenVar* token)
{
    switch (token->type)
    {
    case SERIAL_NUM_VAR:
        sprintf(buff, "SERIAL_NUM_VAR");
        break;
    case FACTORY_NUM_VAR:
        sprintf(buff, "FACTORY_NUM_VAR");
        break;
    case DIR_NAME_VAR:
        sprintf(buff, "DIR_NAME_VAR");
        break;
    case ENG_NAME_VAR:
        sprintf(buff, "ENG_NAME_VAR");
        break;
    case CONS_PLAN_VAR:
        sprintf(buff, "CONS_PLAN_VAR");
        break;
    case CONS_REAL_VAR:
        sprintf(buff, "CONS_REAL_VAR");
        break;

    case OPEN_BRACKET:
        sprintf(buff, "OPEN_BRACKET");
        break;
    case CLOSE_BRAKET:
        sprintf(buff, "CLOSE_BRAKET");
        break;

    case EQUAL_SIGN:
        sprintf(buff, "EQUAL_SIGN");
        break;

    case INT_TYPE:
        sprintf(buff, "INT_TYPE: %d", token->value.intValue);
        break;
    case FLOAT_TYPE:
        sprintf(buff, "INT_TYPE: %f", token->value.floatValue);
        break;
    case STRING_TYPE:
        sprintf(buff, "INT_TYPE: %s", token->value.stringValue);
        break;

    default:
        sprintf(buff, "Unexpected token");
    }
    return buff;
}