#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "theme.h"
#include "utils.h"

void draw_screen_border(WINDOW *window);

void rectangle(int x, int y, int size_x, int size_y);

void fill_rectangle(int x, int y, int size_x, int size_y);

void print_valid_input(int high_score, int high_score_time);

void print_time_date(int x, int y);

void print_high_score(game_stats game_stats, int x, int y);

void print_score_time(game_stats game_stats, int x, int y);

void info_panel(game_stats game_stats, int status);

void draw_menu(WINDOW *window, menu main_menu, int selected);

void draw_theme_menu(WINDOW *window, theme themes[], int theme_count, int selected);

void draw_game(WINDOW *window, int game[][5]);

void draw_end_game(int game_status, int new_high_score);

#endif