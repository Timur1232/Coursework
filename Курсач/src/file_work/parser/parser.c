#include "parser.h"

#include <string.h>

static const char* const VarTokens[] = {
	"SerialNumber",
	"FactoryNumber",
	"DirectorName",
	"EngineerName",
	"EnergyConsPlan",
	"EnergyConsReal"
};


ParserHandler init_parser()
{
	ParserHandler parser;

	parser.shouldClose = 0;
	parser.buffSizeExceeded = 0;
	parser.buff[0] = '\0';
	parser.token = NONE;
	parser.input = init_input();
	parser.scanValue.intValue = 0;

	return parser;
}

InputObserver init_input()
{
	InputObserver input = { 0, 0, 0, 0, 0, 0 };
	return input;
}


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


Token get_token(char* buff)
{
	ignore_white_space(buff);

	int shift = 0;
	Token returnValue = NONE;

	if (buff[0] == '{')
	{
		shift = 1;
		returnValue = OPEN_BRACKET;
	}
	else if (buff[0] == '}')
	{
		shift = 1;
		returnValue = CLOSE_BRAKET;
	}
	else if (buff[0] == '=')
	{
		shift = 1;
		returnValue = EQUAL_SIGN;
	}
	else if (buff[0] == '#')
	{
		shift = 1;
		returnValue = COMMENT;
	}
	else if (buff[0] == '\"')
	{
		shift = 1;
		returnValue = QUOTE_MARK;
	}
	else if (is_digit(buff[0]))
	{
		if (contain(buff, '.'))
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
		return SPEC;

	}
	return NONE_TYPE;
}


int eob(char* buff)
{
	return buff[0] == '\n' || buff[0] == '\0' || buff[0] == -1;
}

int white_space(char ch)
{
	return ch == ' ' || ch == '\n' || ch == '\t';
}

int is_digit(char ch)
{
	return ch >= '0' && ch <= '9';
}

int contain(char* str, char ch)
{
	while (*str != '\0')
	{
		if (*(str++) == ch)
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
