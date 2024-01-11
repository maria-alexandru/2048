#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "theme.h"
#define MAX_OPTIONS 10
#define MAX_OPTION_LENGHT 30
#define SCORES 8
#define MAX_DIM 6

typedef struct {
    int score;
    int time;
    char player[20];
	int status;
}top_score;

typedef struct{
    int option_count;
    char options[MAX_OPTIONS][MAX_OPTION_LENGHT];
	int selected;
}menu;

typedef struct{
    int game[MAX_DIM][MAX_DIM];
    int in_progress; // 0 or 1
	int status; // 0 for game in progress, 1 for win, -1 for loss
	int score;
	int playing_time; // in seconds
	int high_score;
	int high_score_time; // in seconds
	char player[20];
	int old_game[MAX_DIM][MAX_DIM];
	int old_score;
	top_score top_scores[SCORES];
}game_stats;

typedef struct {
	int size;
	int auto_move_sec;
	game_stats crt_game, games[MAX_DIM]; 
    int theme_id;
	int theme_count;
	theme themes[10];
}app_info;

int resize(int *max_x, int *max_y);

char* timestr(struct tm t, char* time);

char* datestr(struct tm t, char* date);

void int_to_string(char *s, int val, int format);

int val_color_id(int val);

char *timestr_sec(int sec, char *time);

void center_text(char s[], int length);

#endif