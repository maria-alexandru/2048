#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#define MAX_OPTIONS 10
#define MAX_OPTION_LENGHT 30

typedef struct{
	int game[5][5];
    int game_in_progress;
	int score;
	int playing_time;
	int high_score;
	int high_score_time;
    int theme_id;
	char player[50];
	int old_game[5][5];
	int old_score;
}game_stats;

typedef struct{
    int option_count;
    char options[MAX_OPTIONS][MAX_OPTION_LENGHT];
}menu;

char* timestr(struct tm t, char* time); // create a string with current time

char* datestr(struct tm t, char* date); // create a string with current date

void int_to_string(char *s, int val, int format);

int val_color_id(int val); // convert cell value to its color id

char *timestr_sec(int sec, char *time);

#endif