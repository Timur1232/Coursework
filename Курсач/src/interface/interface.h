#ifndef CONSOLE_INTERFACE_H
#define CONSOLE_INTERFACE_H

typedef struct ivec2
{
	int x;
	int y;
} ivec2;

typedef struct Panel
{
	int width;
	int height;
	ivec2 pos;
	char** content;
} Panel;

void draw_panel(Panel* panel);

#endif // CONSOLE_INTERFACE_H