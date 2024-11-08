#include "user_input.h"

#include <curses.h>
#include <string.h>

#include <macros.h>
#include <types.h>
#include "../log/log.h"

char* input_str(WINDOW* win, int x, int y)
{
    char* buff = NEW(char, BUFFER_CAPASITY);
    if (!buff)
    {
        LOG_DEBUG(ERR, "user_input.c", "input_str()", "buff allocation error", LOG_FILE);
        exit(-1);
    }
    memset(buff, 0, BUFFER_CAPASITY);
    int size = 0;
    int capasity = BUFFER_CAPASITY;

    wmove(win, y, x);
    int ch = 0;
    while (true)
    {
        ch = wgetch(win);
        if (ch == '\n')
        {
            break;
        }
        if (ch == 224)
        {
            size = clamp(size - 1, 0, capasity);
            wdelch(win);
            buff[size] = '\0';
        }
        if (size == capasity - 1)
        {
            capasity += BUFFER_CAPASITY;
            char* reallocBuff = realloc(buff, capasity);
            if (!reallocBuff)
            {
                free(buff);
                LOG_DEBUG(ERR, "user_input.c", "input_str()", "buff reallocation error", LOG_FILE);
                exit(-1);
            }
            buff = reallocBuff;
        }
        if (ch != KEY_BACKSPACE)
        {
            buff[size++] = ch;
            buff[size] = '\0';
        }
        mvwprintw(win, y, x, buff);
        mvwprintw(win, y, x, buff);
        wrefresh(win);
    }
    return buff;
}
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
			*dest *= 10;
			*dest += digitChar;
		}
		else
		{
			return -1;
		}
	}
	if (negative) *dest *= -1;

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