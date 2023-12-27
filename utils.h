#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define MAX_OPTIONS 10
#define MAX_OPTION_LENGHT 30
#define SCORES 8

typedef struct {
    int score;
    int time;
    char player[20];
}top_score;

typedef struct {
	int game[5][5];
    int game_in_progress; // 0 or 1
	int score;
	int playing_time; // in seconds
	int high_score;
	int high_score_time; // in seconds
    int theme_id;
	char player[20];
	int old_game[5][5];
	int old_score;
	int game_status; // 0 for game in progress, 1 for win, -1 for loss
	top_score top_scores[SCORES];
}game_stats;

typedef struct{
    int option_count;
    char options[MAX_OPTIONS][MAX_OPTION_LENGHT];
}menu;

int resize(int *max_x, int *max_y);

char* timestr(struct tm t, char* time); // create a string with current time

char* datestr(struct tm t, char* date); // create a string with current date

void int_to_string(char *s, int val, int format);

int val_color_id(int val); // convert cell value to its color id

char *timestr_sec(int sec, char *time);

void center_text(char s[], int length);

#endif