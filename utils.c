#include "utils.h"

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
