#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <curses.h>

#define BUFFER_CAPASITY 50

// nodiscard
char* input_str(WINDOW* win, int x, int y);

#endif