#include "tokenizer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../macros.h"

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

/*=====================================[Токенайзер]=====================================*/

ErrorHandler tokenize(TokenQueue* tokens, FILE* file)
{
	clear_tokens(tokens);

	char buff[BUFFER_SIZE] = { 0 };
	int openBracket = 0;
	RepeatObserver observer = init_observer();
	ErrorHandler error = { 0, ALL_GOOD };

	while (fgets(buff, BUFFER_SIZE, file) != NULL)
	{
		if (ferror(file))
		{
			error.err = FILE_ERROR;
			return error;
		}
		if (exceded_buff(buff))
		{
			error.err = BUFF_SIZE_EXCEEDED;
			return error;
		}

		error.line++;

		while (!eob(buff))
		{
			TokenVar token = init_token();
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
				return error;
			}
			else if (token.type == OPEN_BRACKET)
			{
				if (openBracket)
				{
					error.err = NO_CLOSE_BRACKET;
					return error;
				}
				openBracket = 1;
			}
			else if (token.type == CLOSE_BRAKET)
			{
				if (!openBracket)
				{
					error.err = NO_OPEN_BRACKET;
					return error;
				}
				openBracket = 0;
				observer = init_observer();
			}
			else if (token_type(token.type) == VAR)
			{
				if (!openBracket)
				{
					error.err = NO_OPEN_BRACKET;
					return error;
				}
				if (check_repeat(&observer, token.type))
				{
					error.err = MULTIPLE_VARS;
					return error;
				}
				if (token_type(scan_token(buff)) != ASSIGN)
				{
					error.err = EXPECT_ASSIGN;
					return error;
				}
				error.err = get_value(buff, &token);
				if (error.err != ALL_GOOD)
				{
					return error;
				}
			}

			push_token(tokens, &token);
		}
	}
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

	int shift = 0;
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
	else if (my_strcmp(VarTokens[SERIAL_NUM], buff) == 0)
	{
		shift = strlen(VarTokens[SERIAL_NUM]);
		returnValue = SERIAL_NUM;
	}
	else if (my_strcmp(VarTokens[FACTORY_NUM], buff) == 0)
	{
		shift = strlen(VarTokens[FACTORY_NUM]);
		returnValue = FACTORY_NUM;
	}
	else if (my_strcmp(VarTokens[DIR_NAME], buff) == 0)
	{
		shift = strlen(VarTokens[DIR_NAME]);
		returnValue = DIR_NAME;
	}
	else if (my_strcmp(VarTokens[ENG_NAME], buff) == 0)
	{
		shift = strlen(VarTokens[ENG_NAME]);
		returnValue = ENG_NAME;
	}
	else if (my_strcmp(VarTokens[CONS_PLAN], buff) == 0)
	{
		shift = strlen(VarTokens[CONS_PLAN]);
		returnValue = CONS_PLAN;
	}
	else if (my_strcmp(VarTokens[CONS_REAL], buff) == 0)
	{
		shift = strlen(VarTokens[CONS_REAL]);
		returnValue = CONS_REAL;
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
	case SERIAL_NUM:
	case FACTORY_NUM:
	case DIR_NAME:
	case ENG_NAME:
	case CONS_PLAN:
	case CONS_REAL:
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
	case SERIAL_NUM:
	case FACTORY_NUM:
		return INT_TYPE;
	case DIR_NAME:
	case ENG_NAME:
		return STRING_TYPE;
	case CONS_PLAN:
	case CONS_REAL:
		return FLOAT_TYPE;
	default:
		return NONE;
	}
}

/*=====================================[Буфер]=====================================*/

void shift_buff(char* buff, int n)
{
	if (!n) return;
	int size = 0;
	for (int i = n, j = 0; i < BUFFER_SIZE && buff[i]; i++, j++)
	{
		buff[j] = buff[i];
		size++;
	}
	buff[size] = '\0';
}

void ignore_white_space(char* buff)
{
	int count = 0;
	for (int i = 0; i < BUFFER_SIZE && white_space(buff[i]); i++)
	{
		count++;
	}
	shift_buff(buff, count);
}

/*=====================================[Логические функции]=====================================*/

int exceded_buff(char* buff)
{
	return buff[strlen(buff) - 1] != '\n';
}

int eob(char* buff)
{
	return buff[0] == '\n' || buff[0] == '\0' || buff[0] == -1;
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

int my_strcmp(const char* str1, const char* str2)
{
	while (*str1 == *str2)
	{
		str1++;
		str2++;
	}
	if (*str1 == '\0') return 0;
	return *str1 - *str2;
}

int check_repeat(RepeatObserver* observer, Token token)
{
	switch (token)
	{
	case SERIAL_NUM:
	{
		if (observer->serialNumber)
			return 1;
		observer->serialNumber = 1;
		break;
	}
	case FACTORY_NUM:
	{
		if (observer->factoryNumber)
			return 1;
		observer->factoryNumber = 1;
		break;
	}
	case DIR_NAME:
	{
		if (observer->directorFullName)
			return 1;
		observer->directorFullName = 1;
		break;
	}
	case ENG_NAME:
	{
		if (observer->engineerFullName)
			return 1;
		observer->engineerFullName = 1;
		break;
	}
	case CONS_PLAN:
	{
		if (observer->energyConsPlan)
			return 1;
		observer->energyConsPlan = 1;
		break;
	}
	case CONS_REAL:
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

int get_int(char* buff, int* data)
{
	int count = 0;
	for (int i = 0; i < BUFFER_SIZE && !white_space(buff[i]) && !divider(buff[i]); i++, count++);

	if (sscanf(buff, "%d", data) == 1)
		return count;

	return 0;
}

int get_float(char* buff, float* data)
{
	int count = 0;
	for (int i = 0; i < BUFFER_SIZE && !white_space(buff[i]) && !divider(buff[i]); i++, count++);

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
	int count = 0;
	ignore_white_space(buff);
	// Считывание значения соответствующего типу токена
	switch (var_type(token->type))
	{
	case INT_TYPE:
		if (scan_token(buff) != INT_TYPE)
		{
			return EXPECT_INT;
		}
		count = get_int(buff, &token->value.intValue);
		break;
	case FLOAT_TYPE:
		if (scan_token(buff) != FLOAT_TYPE)
		{
			return EXPECT_FLOAT;
		}
		count = get_float(buff, &token->value.floatValue);
		break;
	case STRING_TYPE:
		if (scan_token(buff) != QUOTE_MARK)
		{
			return EXPECT_STR;
		}
		count = get_str(buff, token->value.stringValue);
		break;
	default:
		return EXPECT_VALUE;
	}
	if (!count)
	{
		switch (var_type(token->type))
		{
		case INT_TYPE:
			return SCAN_INT_ERR;
		case FLOAT_TYPE:
			return SCAN_FLOAT_ERR;
		case STRING_TYPE:
			return SCAN_STR_ERR;
		}
		return EXPECT_VALUE;
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
		sprintf(buff, "File open erron");
		break;
	case FILE_ERROR:
		sprintf(buff, "File erron");
		break;
	case BUFF_SIZE_EXCEEDED:
		sprintf(buff, "Buffer size exceeded; line: %d", error.line);
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
	case SERIAL_NUM:
		sprintf(buff, "SERIAL_NUM: %d", token->value.intValue);
		break;
	case FACTORY_NUM:
		sprintf(buff, "FACTORY_NUM: %d", token->value.intValue);
		break;
	case DIR_NAME:
		sprintf(buff, "DIR_NAME: %s", token->value.stringValue);
		break;
	case ENG_NAME:
		sprintf(buff, "ENG_NAME: %s", token->value.stringValue);
		break;
	case CONS_PLAN:
		sprintf(buff, "CONS_PLAN: %f", token->value.floatValue);
		break;
	case CONS_REAL:
		sprintf(buff, "CONS_REAL: %f", token->value.floatValue);
		break;

	case OPEN_BRACKET:
		sprintf(buff, "OPEN_BRACKET");
		break;
	case CLOSE_BRAKET:
		sprintf(buff, "CLOSE_BRAKET");
		break;

	default:
		sprintf(buff, "Unexpected token");
	}
	return buff;
}