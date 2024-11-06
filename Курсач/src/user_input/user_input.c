#include "user_input.h"

#include <curses.h>

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
