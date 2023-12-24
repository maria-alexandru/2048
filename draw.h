#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

void init(); // initialize

void draw_screen_border(WINDOW *window);

void rectangle(int x, int y, int size_x, int size_y);

void print_valid_input(int x, int y);

void info_panel(int score, int status);

void draw_menu(WINDOW *window, char options[][20], int option_count, int selected);

void draw_game(WINDOW *window, int game[][5]);

#endif