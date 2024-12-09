#ifndef USER_INPUT_H
#define USER_INPUT_H

#include <curses.h>

#define BUFFER_CAPASITY 50

int parse_int(const char* str, int* dest);
int parse_float(const char* str, float* dest);

#endif