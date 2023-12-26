#include "utils.h"

// refresh the screen if terminal window size changes
int resize(int *max_x, int *max_y)
{
	int new_max_x, new_max_y;
	getmaxyx(stdscr, new_max_y, new_max_x);
	if (new_max_x != *max_x || new_max_y != *max_y) {
		getmaxyx(stdscr, *max_y, *max_x);
		refresh();
		return 1;
	}
	return 0;
}

// create a string with current time
char* timestr(struct tm t, char* time)
{
    char nr[5];
    time[0]='\0';
    int_to_string(nr, t.tm_hour, 2);
	if (t.tm_hour < 10) {
		nr[2] = '\0';
		nr[1] = nr[0];
		nr[0] = '0';
	}
    strcat(time, nr);
    strcat(time, ":");
    int_to_string(nr, t.tm_min, 2);
	if (t.tm_min < 10) {
		nr[2] = '\0';
		nr[1] = nr[0];
		nr[0] = '0';
	}
    strcat(time, nr);
    strcat(time, ":");
    int_to_string(nr, t.tm_sec, 2);
	if (t.tm_sec < 10) {
		nr[2] = '\0';
		nr[1] = nr[0];
		nr[0] = '0';
	}
    strcat(time, nr);
    return time;
}

// create a string with current date
char* datestr(struct tm t, char* date)
{
    char nr[10];
    date[0]='\0';
    int_to_string(nr, t.tm_mday, 2);
	if (t.tm_mday < 10) {
		nr[2] = '\0';
		nr[1] = nr[0];
		nr[0] = '0';
	}
    strcat(date, nr);
    strcat(date, ".");
    int_to_string(nr, t.tm_mon + 1, 2);
	if (t.tm_mon < 10) {
		nr[2] = '\0';
		nr[1] = nr[0];
		nr[0] = '0';
	}
    strcat(date, nr);
    strcat(date, ".");
    int_to_string(nr, 1900 + t.tm_year, 4);
    strcat(date, nr);
    return date;
}

void int_to_string(char *s, int val, int format)
{
	int i = 0, count = 0;
	int copy = val;
	do {
		copy = copy/10;
		count++;
	}while (copy != 0);
	if (count <= format) {
		for (i = 0; i < format; i++)
			s[i] = ' ';	
		for (i = (format - count) / 2 + count - 1; i >= (format - count) / 2; i--) {
			s[i] = val % 10 + '0';
			val = val / 10;
		}
		s[format] = '\0';
	} else {
		for (i = count - 1; i >= 0; i--) {
			s[i] = val % 10 + '0';
			val = val / 10;
		}
		s[count] = '\0';
	}	
}

// convert cell value to its color id
int val_color_id(int val)
{
	int i = 20;
	while (val != 2) {
		i++;
		val = val/2;
	}
	return i;
}

char *timestr_sec(int sec, char *time)
{
	struct tm t;
	t.tm_hour = sec / 3600;
	t.tm_min = (sec - t.tm_hour * 3600) / 60;
	t.tm_sec = sec - t.tm_min * 60 - t.tm_hour * 3600;
	timestr(t, time);
}

void center_text(char s[], int length)
{
	int i, s_len;
	char copy[50];
	s_len = strlen(s);
	for (i = (length - s_len) / 2; i < (length - s_len) / 2 + s_len; i++)
		copy[i] = s[i - (length - s_len) / 2];
	
	for (i = 0; i < (length - s_len) / 2; i++)
		copy[i] = ' ';

	for (i =  (length - s_len) / 2 + s_len; i < length; i++)
		copy[i] = ' ';
	
	copy[length] = '\0';
	strcpy(s, copy);	
}