#include "user_input.h"

#include <curses.h>
#include <string.h>
#include <limits.h>

#include <macros.h>
#include <types.h>
#include "../log/log.h"

int parse_int(const char* str, int* dest)
{
	*dest = 0;
	int strLen = strlen(str);
	// Основной цикл ввода
	Bool negative = false;
	for (int i = 0; i < strLen; i++)
	{
		int digitChar = str[i];
		// Условие выхода
		if (digitChar == '\n' || digitChar == ' ')
		{
			if (*dest != 0)
			{
				return -1;
			}
			continue;
		}
		if (digitChar == '-')
		{
			if (negative || *dest != 0)
			{
				return -1;
			}
			negative = true;
			continue;
		}
		digitChar -= '0';	// Получение цифры из введенного символа
		if (digitChar >= 0 && digitChar <= 9)
		{
			if (*dest * 10 <= *dest && *dest != 0)
			{
				*dest = INT_MAX;
				break;
			}
			*dest *= 10;
			*dest += digitChar;
		}
		else
		{
			return -1;
		}
	}
	if (negative)
	{
		*dest *= -1;
		if (*dest == -INT_MAX) (*dest)--;
	}

	return 0;
}

int parse_float(const char* str, float* dest)
{
	*dest = 0.0f;

	float decimalMult = 0.1f;
	Bool negative = false;
	Bool period = false;
	int strLen = strlen(str);
	// Основной цикл ввода
	for (int i = 0; i < strLen; i++)
	{
		int digitChar = str[i];
		if (digitChar == '\n' || digitChar == ' ')
		{
			if (*dest != 0.0f)
			{
				return -1;
			}
			continue;
		}
		if (digitChar == '-')
		{
			if (negative || *dest != 0.0f)
			{
				return -1;
			}
			negative = true;
			continue;
		}
		if (digitChar == '.' || digitChar == ',')
		{
			if (period) {
				return -1;
			}
			period = true;
			continue;
		}

		digitChar -= '0';
		if (digitChar >= 0 && digitChar <= 9)
		{
			// Дробная часть
			if (period)	
			{
				*dest += (float)digitChar * decimalMult;
				decimalMult *= 0.1f;
			}
			// Целая часть
			else
			{
				*dest *= 10.0f;
				*dest += digitChar;
			}
		}
		else
		{
			return -1;
		}
	}
	if (negative) *dest *= -1;

	return 0;
}