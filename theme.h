#ifndef THEMES_h
#define THEMES_h

#include <stdio.h>
#include <curses.h>

typedef struct {
	char name[30];
	struct color_info {
		int id;
		int r, g, b;
	} color[15];
} theme_info;

int read_themes(theme_info themes[]);

void initialize_colors(theme_info theme);

void set_theme(theme_info theme);

#endif
