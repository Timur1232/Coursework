﻿#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

#include <curses.h>

#include <types.h>
#include "../../list/list.h"
#include "../../ref_array/ref_array.h"
#include "../application/application.h"

#define CHUNCK_SIZE_FULL 16
#define CHUNCK_SIZE_REDACTOR 9

#define SCREEN_WIDTH 144
#define SCREEN_HEIGHT 40

#define CONTROLS_HEIGHT 13

#define BROWSING_MENU_WIN_WIDTH 27
#define BROWSING_MENU_WIN_HEIGHT (SCREEN_HEIGHT - CONTROLS_HEIGHT)

#define TABLE_WIN_WIDTH 117
#define TABLE_WIN_HEIGHT SCREEN_HEIGHT
#define TABLE_WIN_X BROWSING_MENU_WIN_WIDTH
#define TABLE_WIN_Y 0

#define REDACTOR_MENU_WIN_WIDTH 117
#define REDACTOR_MENU_WIN_HEIGHT 15
#define REDACTOR_MENU_WIN_X BROWSING_MENU_WIN_WIDTH
#define REDACTOR_MENU_WIN_Y 0

#define POP_UP_Y (SCREEN_HEIGHT / 2 + SCREEN_HEIGHT / 4 + SCREEN_HEIGHT / 8)

#define BUFFER_CAPASITY 50

// Коды клавиш на клавиатуре
#define MY_KEY_CTRL_A 1
#define MY_KEY_CTRL_C 3
#define MY_KEY_CTRL_D 4
#define MY_KEY_CTRL_F 6
#define MY_KEY_CTRL_G 7
#define MY_KEY_BACKSPACE 8
#define MY_KEY_SAVE 19
#define MY_KEY_CTRL_V 22
#define MY_KEY_REDO 25
#define MY_KEY_UNDO 26
#define MY_KEY_ESCAPE 27
#define MY_KEY_HOME 262
#define MY_KEY_F(n) (264 + n)
#define MY_KEY_DEL 330
#define MY_KEY_INS 331
#define MY_KEY_PGDN 338
#define MY_KEY_PGUP 339
#define MY_KEY_END 358

// Элементы интерфейса
void print_menu(WINDOW* win, const Menu* menu);
void print_table_list(WINDOW* win, ListPtr list, int selected, Focus mode);
void print_table_ref(WINDOW* win, RefArrayPtr entries, int selected, Focus mode);
void print_note_editor(WINDOW* win, FECNotePtr note, int field, int index);
void print_controls(WINDOW* win, Focus type);

// Всплывающее уведомление
void pop_up_notification_wchar(WINDOW* win, const wchar_t* message, NotificationType type, int y);
void pop_up_notification(WINDOW* win, const char* message, NotificationType type, int y);

typedef enum InputType
{
    INPUT_STRING,
    INPUT_NUMBER
} InputType;

/**
* Получение пользовательского ввода с клавиатуры
*/
// [[nodiscard]]
char* get_user_input_str(WINDOW* win, const wchar_t* message, int y, InputType type);
int get_user_input_int(WINDOW* win, const wchar_t* message, int* dest);
int get_user_input_float(WINDOW* win, const wchar_t* message, float* dest);

#endif // CONSOLE_INTERFACE_H