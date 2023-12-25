#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"
#include "movement.h"
#include "draw.h"
#include "theme.h"

int init()
{
	// check if colors are availabe
	if (!(has_colors() && start_color() == OK)) {
		endwin();
		printf("No colors available. Exit");
		return -1;
	}

	noecho(); // no output when characters are typed
	keypad(stdscr, TRUE); // activate keyboard
	cbreak(); // no buffering	
	curs_set(0); // hide cursor
	srand(time(NULL));

	refresh();
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);
	bkgd(COLOR_PAIR(1));
	return 1; // successfully initialized
}

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

void generate_rand_cell(int game[][5]) 
{
	typedef struct {
		int i, j;
	}free_cell;
	free_cell free_cells[20];
	int free = 0;
	int i, j;
	int rand_cell, rand_val;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game[i][j] == 0) {
				free_cells[free].i = i;
				free_cells[free++].j = j;
			}
	if (free != 0) {
		rand_cell = rand() % free; // generate a random index for the free cell
		rand_val = (rand() % 2 + 1) * 2; // generate a random value
		game[free_cells[rand_cell].i][free_cells[rand_cell].j] = rand_val;
	}
}

int check_winner(int game[][5])
{
	int i,j;
	int move = auto_move(game);

	if (move == '\0')
		return -1; 	// lose

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game[i][j] == 2048)
				return 1; // win

	// continue game
	return 0;
}

void start_game(WINDOW *window, int *game_in_progress, int game[][5], int *score,
				int *playing_time)
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int key;
	int valid;
	time_t start_time, curr_time;
	int timeout_sec = 10;
	int game_status;
	int playing_time_aux = *playing_time;

	if (*game_in_progress == 0) {
		*game_in_progress = 1;
		generate_rand_cell(game);
		generate_rand_cell(game);
	}

	// draw game layout
	draw_game(window, game);
	start_time = time(NULL);
	nodelay(stdscr, TRUE);
	while(1) {
		if (resize(&max_x, &max_y) == 1)
			draw_game(window, game);

		game_status = check_winner(game);
		if (game_status == 1)
			break;
		else if (game_status == -1)
			break;

		curr_time = time(NULL);
		*playing_time = playing_time_aux + (int)difftime(curr_time, start_time);
		info_panel(*score, game_status, *playing_time);
		valid = 0;
		key = getch();
		// if there was no input for timeout_sec seconds, move automatically		
		if (difftime(curr_time, start_time) >= timeout_sec)
			key = auto_move(game);
		if (key > 0) {
			playing_time_aux = *playing_time;
			start_time = time(NULL);
			if (key == 'Q')
				break;
			else if (key == KEY_DOWN)
				valid = move_down(game, score);
			else if (key == KEY_UP)
				valid = move_up(game, score);
			else if (key == KEY_LEFT)
				valid = move_left(game, score);
			else if (key == KEY_RIGHT)
				valid = move_right(game, score);

			if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT ||
				key == KEY_LEFT) {
				if (valid == 1)
					generate_rand_cell(game);
				draw_game(window, game);
			}
		}
	}
	if (game_status != 0) {
		clear();
		draw_game(window, game);
		// wait until Q is pressed to exit
		while (1) {
			info_panel(*score, game_status, *playing_time);
			key = getch();
			if (key == 'Q')
				break;
		}
	}
    nodelay(stdscr, FALSE);
}

void reset_game(int *game_in_progress, int game[][5], int *score, int *playing_time)
{
	int i, j;
	*game_in_progress = 0;
	*playing_time = 0;
	*score = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game[i][j] = 0;
}

void open_theme_menu(WINDOW *window, theme themes[], int theme_count)
{
	int cursor_pos_x = 0, cursor_pos_y = 0;
	int max_x, max_y;
	int key, selected = 0;

	// get max size of the window
	getmaxyx(stdscr, max_y, max_x);
	// move the cursor to the center so that the text will be centered
	move(cursor_pos_y, cursor_pos_x);
	
	while (1) {
		resize(&max_x, &max_y);
		draw_theme_menu(window, themes, theme_count, selected);
		key = getch();
		if (key == KEY_DOWN && selected < theme_count - 1)
			selected++;
		if (key == KEY_UP && selected > 0)
			selected--;
		if (key == 'Q')
			break;
		
		// enter is pressed
		if (key == '\n') {
			set_theme(themes[selected]);
			break;
		}
	}
}

int main()
{
	WINDOW *window = initscr();
	int game[5][5] = {0};
	int game_in_progress = 0, score = 0, playing_time = 0;
	int option_count = 4, selected = 0;
	int key;
	int max_x, max_y;
	char options[5][20] = {"New Game", "Resume", "Theme", "Quit"}; // menu options
	theme themes[10];
	int theme_count = 0;

	if (init() == -1)
		return 0;

	getmaxyx(stdscr, max_y, max_x);
	theme_count = read_themes(themes);
	set_theme(themes[0]);
	while(1) {
		resize(&max_x, &max_y); // refresh if terminal window size changes
		draw_menu(window, options, option_count, selected);
		key = getch();
		if (key == KEY_DOWN && selected < option_count - 1)
			selected++;
		if (key == KEY_UP && selected > 0)
			selected--;
		if (strcmp(options[selected], "Resume") == 0 && game_in_progress == 0) {
			if (key == KEY_DOWN)
				selected++;
			else if (key == KEY_UP)
				selected--;
		}
		// enter is pressed
		if (key == '\n') {
			if (strcmp(options[selected], "Quit") == 0) {
				clear();
				//reset_game(&game_in_progress, game, &score);
				break;
			} else if (strcmp(options[selected], "New Game") == 0) {
				move(0, 0);
				reset_game(&game_in_progress, game, &score, &playing_time);
				start_game(window, &game_in_progress, game, &score, &playing_time);
				clear();
			} else if (strcmp(options[selected], "Resume") == 0) {
				if (game_in_progress == 1) {
					move(0, 0);
					start_game(window, &game_in_progress, game, &score, &playing_time);
					clear();
				}
			} else if (strcmp(options[selected], "Theme") == 0) {
				open_theme_menu(window, themes, theme_count);
				clear();
			}
		}
		clear();
	}
	delwin(window);
	
	endwin();
	return 0;
}