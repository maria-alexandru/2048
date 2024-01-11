#ifndef DRAW_H
#define DRAW_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "theme.h"
#include "utils.h"
#include "movement.h"

void draw_logo(WINDOW *window);

void draw_screen_border(WINDOW *window);

void rectangle(int x, int y, int size_x, int size_y);

void fill_rectangle(int x, int y, int size_x, int size_y);

void print_valid_input(int x, int y, app_info app);

void print_time_date(int x, int y);

void print_high_score(game_stats_info game_stats, int x, int y);

void print_score_time(game_stats_info game_stats, int x, int y);

void info_panel(app_info app);

void draw_menu(WINDOW *window, menu_info main_menu, int selected);

void draw_theme_menu(WINDOW *window, theme_info themes[], int theme_count,
					 int selected);

void draw_game(WINDOW *window, int game[][MAX_DIM], int size);

void draw_hs_message(game_stats_info *game_stats, char name[], int size);

void draw_end_game(int game_status, int size);

void draw_top_scores(WINDOW *window, top_score top_scores[]);

void draw_auto_move(int auto_move_sec, int selected);

#endif
