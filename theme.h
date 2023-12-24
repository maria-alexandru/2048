#ifndef THEMES_h
#define THEMES_h

#include <stdio.h>
#include <curses.h>

typedef struct {
    char name[30];
    struct Color{
        int id;
        int r, g, b;
    }color[15];
}theme;

int read_themes(theme themes[]);

void initialize_colors(theme theme);

void set_theme(theme theme);

#endif