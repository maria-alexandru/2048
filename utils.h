#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <curses.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>

char* timestr(struct tm t, char* time); // create a string with current time

char* datestr(struct tm t, char* date); // create a string with current date

void int_to_string(char *s, int val, int format);

int val_color_id(int val); // convert cell value to its color id

#endif