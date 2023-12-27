#include "draw.h"
#define LINE_COLOR 18
#define TEXT_COLOR 19
#define R_EDGE 25

void draw_screen_border(WINDOW *window)
{
	attron(COLOR_PAIR(2));
	attron(A_STANDOUT);
	box(window, 0, 0); // border
	attroff(A_STANDOUT);
	attroff(COLOR_PAIR(2));
	wrefresh(window);
}

void rectangle(int x, int y, int size_x, int size_y)
{
	mvhline(y, x, 0, size_x);
	mvvline(y, x, 0, size_y);
	mvvline(y, x + size_x, 0, size_y);
	mvhline(y + size_y, x, 0, size_x);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + size_y, x, ACS_LLCORNER);
    mvaddch(y, x + size_x, ACS_URCORNER);
    mvaddch(y + size_y, x + size_x, ACS_LRCORNER);
}

void fill_rectangle(int x, int y, int size_x, int size_y)
{
	int i = x;
	for (x; x < i + size_x; x++) {
		mvvline(y, x, 0, size_y);
	}
}

void print_valid_input(int x, int y)
{
	mvaddstr(y, x, "Valid input: ");
	y++;
	mvaddch(y, x, ACS_UARROW);
	mvaddstr(y, x + 1, " - move up");
	y++;
	mvaddch(y, x, ACS_DARROW);
	mvaddstr(y, x + 1, " - move down");
	y++;
	mvaddch(y, x, ACS_LARROW);
	mvaddstr(y, x + 1, " - move left");
	y++;
	mvaddch(y, x, ACS_RARROW);
	mvaddstr(y, x + 1, " - move right");
	y++;
	mvaddstr(y, x, "Q - quit");
	y++;
	mvaddstr(y, x, "U - undo");
}

void print_time_date(int x, int y)
{
	char date[20], time_now[20];
	struct tm *time_p;

	time_t t = time(NULL); // get time
	time_p = localtime(&t);
	timestr(*time_p, time_now);
	datestr(*time_p, date);
	mvaddstr(y, x, date);
	mvaddstr(y + 1, x, time_now);
}

void print_high_score(game_stats game_stats, int x, int y)
{
	char HS_time[20], HS[20];
	timestr_sec(game_stats.high_score_time, HS_time);
	int_to_string(HS, game_stats.high_score, 0);
	mvaddstr(y, x, "High score: ");
	mvaddstr(y, x + strlen("High score: "), HS);
	mvaddstr(y + 1, x, "High score time: ");
	mvaddstr(y + 2, x, HS_time + 3);
	mvaddstr(y + 3, x, "Player: ");
	mvaddstr(y + 4, x, game_stats.player);
}

void print_score_time(game_stats game_stats, int x, int y)
{
	char score_text[20], time_playing[20];
	timestr_sec(game_stats.playing_time, time_playing);
	int_to_string(score_text, game_stats.score, 2);

	mvaddstr(y, x, "Score: ");
	mvaddstr(y, x + strlen("Score: "), score_text);
	mvaddstr(y + 1, x, "Playing: ");
	// print only minutes and seconds of playing time
	mvaddstr(y + 1, x + strlen("Playing: "), time_playing + 3);
}

void info_panel(game_stats game_stats, int status)
{
	int x, y, size_x, size_y;
	getmaxyx(stdscr, size_y, size_x);
	size_x = 21; size_y -= 4;
	if (size_y < R_EDGE)
		size_y = R_EDGE;
	x = 4; y = 3;
	
	attron(COLOR_PAIR(2));
	fill_rectangle(2, 2, size_x , size_y - 1);
	attron(COLOR_PAIR(TEXT_COLOR));
	rectangle(2, 2, size_x, size_y - 1);

	print_time_date(x, y); // print current date and time
	y += 3;
	print_score_time(game_stats, x, y); // print score and playing time
	y += 3;
	print_high_score(game_stats, x, y); // print high score info
	y += 6;//5

	if (status == 0)
		print_valid_input(x, y); // if game is not over, print valid commands
	else if (status == -1)
		mvaddstr(y, x, "Game over!");
	else if (status == 1)
		mvaddstr(y, x, "You won!");
	if (status == 1 || status == -1) {
		y++;
		mvaddstr(y, x, "Press Q to exit");
		y++;
		mvaddstr(y, x, "Press N to start");
		mvaddstr(y + 1, x, "a new game");
	}
}

void draw_game(WINDOW *window, int game[][5])
{
	clear();
	int max_x, max_y;
	int cursor_pos_x, cursor_pos_y;
	int cell_size = 4;
	int layout_size = 4 * cell_size;
	int i, j;
	int x, y;

	//get size of the window
	getmaxyx(stdscr, max_y, max_x);
	cursor_pos_y = (max_y -  layout_size) / 2 ;
	cursor_pos_x = (max_x - 2 * layout_size) / 2;
	if (cursor_pos_x < R_EDGE)
		cursor_pos_x = R_EDGE;
	move(cursor_pos_y, cursor_pos_x);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			x = cursor_pos_x + j * cell_size * 2;
			y = cursor_pos_y + i * cell_size;
			//draw cell
			attron(COLOR_PAIR(LINE_COLOR));
			rectangle(x, y, 2 * cell_size, cell_size);
			//print cell value
			if (game[i][j] != 0) {
				attron(COLOR_PAIR(val_color_id(game[i][j])));
				rectangle(x + 1, y + 1, 2 * cell_size - 2, cell_size - 2);
				x+=2;
				y+=2;
				char val[10]="";
				attron(A_BOLD);
				int_to_string(val, game[i][j], cell_size + 1);
				mvaddstr(y, x, val);
				attroff(A_BOLD);
			}
		}
	}

	//draw game border
	attron(COLOR_PAIR(LINE_COLOR));
	rectangle(cursor_pos_x, cursor_pos_y, 2 * layout_size, layout_size);
	draw_screen_border(window);
}

void draw_menu(WINDOW *window, menu main_menu, int selected)
{
	int cursor_pos_x = 0, cursor_pos_y = 0;
	int max_x, max_y;
	int i;

	// get max size of the window
	getmaxyx(stdscr, max_y, max_x);
	// move the cursor to the center so that the text will be centered
	move(cursor_pos_y, cursor_pos_x);

	cursor_pos_y = (max_y -  3 * main_menu.option_count) / 2 - 2;
	cursor_pos_x = (max_x - 15) / 2;
	for (i = 0; i < main_menu.option_count; i++) {
		// highlight option if it is selected
		if (selected == i)
			attron(A_STANDOUT);
		else
			attroff(A_STANDOUT);
		
		attron(COLOR_PAIR(TEXT_COLOR));
		fill_rectangle(cursor_pos_x, cursor_pos_y, 15, 2);
		rectangle(cursor_pos_x, cursor_pos_y, 15, 2);

		// display options
		char string[30];
		strcpy(string, main_menu.options[i]);
		center_text(string, 14);
		mvaddstr(cursor_pos_y + 1, cursor_pos_x + 1, string);// main_menu.options[i]);
		cursor_pos_y += 4;
		move(cursor_pos_y, cursor_pos_x);
	}	
	draw_screen_border(window);
	wrefresh(window);
}

void draw_theme_menu(WINDOW *window, theme themes[], int theme_count, int selected)
{
	clear();
	int max_x, max_y, x = 10, y = 10, start_x = 3, start_y = 3;
	int i, k;
	int cell_size = 4;
	int key;
	int new_max_x, new_max_y;
	int count = 0;

	// get max size of the window
	getmaxyx(stdscr, max_y, max_x);
	x = start_x;
	y = start_y;
	for (k = 0; k < theme_count; k++) {
		// if theme is selected, highlight it
		if (selected == k)
			attron(COLOR_PAIR(11)); // highlight color
		else
			attron(COLOR_PAIR(12)); // not highlight color
		rectangle(x - 1, y - 1, max_x - 5, cell_size + 2);
		fill_rectangle(x - 1, y - 1, max_x - 5, cell_size + 2);
		attroff(A_STANDOUT);
		if (selected == k)
			attron(COLOR_PAIR(9)); // highlight color
		else
			attron(COLOR_PAIR(10)); // not highlight color
		mvaddstr(y, x, themes[k].name);
		y++;
		count = 0;
		// print cell with its value and color
		for (i = 2; i <= 2048; i = i * 2) {
			attron(COLOR_PAIR(themes[k].color[count].id));
			rectangle(x + 1, y + 1, 2 * cell_size - 2, cell_size - 2);
			char val[10] = "";
			attron(A_BOLD);
			int_to_string(val, i, cell_size + 1);
			mvaddstr(y + 2, x + 2, val);
			x += 2 * cell_size ;
			attroff(A_BOLD);
			count++;
		}
		y += cell_size + 3;
		x = start_x;
	}
	draw_screen_border(window);
	refresh();				
}

// high score or top score message
void draw_hs_message(game_stats *game_stats, char name[])
{
	int max_x, max_y;
	int cursor_pos_x, cursor_pos_y, start_x;
	int cell_size = 4;
	int layout_size = 4 * cell_size;
	int i;
	int length;
	char message[40];
	if (game_stats->score == game_stats->high_score)
		strcpy(message, "   New high score!    ");
	else
		strcpy(message, "    New top score!    ");
	 
	length = strlen(message);

	//get size of the window
	getmaxyx(stdscr, max_y, max_x);
	cursor_pos_y = (max_y -  layout_size) / 2 - 4;
	if(cursor_pos_y < 1)
		cursor_pos_y = 1;
	cursor_pos_y =  (max_y -  layout_size) + 4;
	cursor_pos_x = (max_x - 2 * layout_size) / 2 + layout_size - length / 2;
	if (cursor_pos_x < R_EDGE + layout_size / 2)
		cursor_pos_x = R_EDGE + layout_size / 2;
	move(cursor_pos_y, cursor_pos_x);
	attron(COLOR_PAIR(TEXT_COLOR));
	fill_rectangle(cursor_pos_x, cursor_pos_y, length + 1, 5);
	rectangle(cursor_pos_x, cursor_pos_y, length + 1, 5);
	mvaddstr(cursor_pos_y + 1, cursor_pos_x + 1, message);
	if (game_stats->score == game_stats->high_score)
		strcpy(message, " Save the high score! ");
	else
		strcpy(message, "  Save the top score! ");
	mvaddstr(cursor_pos_y + 2, cursor_pos_x + 1, message);
	strcpy(message, "    Type your name:   ");
	mvaddstr(cursor_pos_y + 3, cursor_pos_x + 1, message);
	strcpy(message, "                      ");
	mvaddstr(cursor_pos_y + 4, cursor_pos_x + 1, message);

	start_x = cursor_pos_x;
	cursor_pos_y += 4;
	cursor_pos_x += length / 2;

	cursor_pos_x = start_x + length / 2 - (strlen(name) - 1) / 2;
	mvaddstr(cursor_pos_y, start_x + 1, message);
	mvaddstr(cursor_pos_y, cursor_pos_x, name);
}

// game over or you won message
void draw_end_game(int game_status)
{
	int max_x, max_y;
	int cursor_pos_x, cursor_pos_y;
	int cell_size = 4;
	int layout_size = 4 * cell_size;
	int length;
	char message[40];
	if(game_status == -1)
		strcpy(message, " Game over! ");
	else 
		strcpy(message, " You won! ");
	length = strlen(message);

	//get size of the window
	getmaxyx(stdscr, max_y, max_x);
	cursor_pos_y = (max_y -  layout_size) / 2 - 4;
	if(cursor_pos_y < 1)
		cursor_pos_y = 1;
	cursor_pos_x = (max_x - 2 * layout_size) / 2 + layout_size - length / 2;
	if (cursor_pos_x < R_EDGE + layout_size / 2)
		cursor_pos_x = R_EDGE + layout_size / 2;
	move(cursor_pos_y, cursor_pos_x);
	attron(COLOR_PAIR(TEXT_COLOR));

	fill_rectangle(cursor_pos_x, cursor_pos_y, length + 1, 2);
	rectangle(cursor_pos_x, cursor_pos_y, length + 1, 2);
	mvaddstr(cursor_pos_y + 1, cursor_pos_x + 1, message);
}

// top scores window
void draw_top_scores(WINDOW *window, top_score top_scores[])
{
    clear();
	int max_x, max_y, x = 10, y = 10;
	char score[20] = "Score";
	char time[20] = "Time";
	char name[20] = "Name";
	int len = 20, k;

	// get max size of the window
	getmaxyx(stdscr, max_y, max_x);
	x = (max_x - 3 * len) / 2;
	y = max_y / SCORES;

	attron(COLOR_PAIR(3));
	fill_rectangle(x, y, max_x - 2 * x, 2);
	attron(COLOR_PAIR(4));
	rectangle(x, y, max_x - 2 * x, 2);
	center_text(score, len);
	center_text(time, len);
	center_text(name, len);
	y++;
	mvaddstr(y, x + 1, score);
	mvaddstr(y, x + len, time);
	mvaddstr(y, x + 2 * len, name);

	for (k = 0; k < SCORES; k++) {
		y += 3;
		move(y, x);
		attron(COLOR_PAIR(2));
		fill_rectangle(x, y, max_x - 2 * x, 2);
		attron(COLOR_PAIR(TEXT_COLOR));
		rectangle(x, y, max_x - 2 * x, 2);
		y++;
		
		int_to_string(score, top_scores[k].score, 0);
		int_to_string(time, top_scores[k].time, 0);
		strcpy(name, top_scores[k].player);
		center_text(score, len);
		center_text(time, len);
		center_text(name, len);
		mvaddstr(y, x + 1, score);
		mvaddstr(y, x + len, time);
		mvaddstr(y, x + 2 * len, name);
	}
	draw_screen_border(window);
	refresh();
}