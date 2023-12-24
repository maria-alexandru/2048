#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"
#include "movement.h"
#include "draw.h"

void start_game(WINDOW *window, int *game_in_progress, int game[][5], int *score);

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
	char move = auto_move(game);

	if (move == '\0')
		return -1; 	// lose

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game[i][j] == 2048)
				return 1; // win

	// continue game
	return 0;
}

void start_game(WINDOW *window, int *game_in_progress, int game[][5], int *score)
{
	int key;
	int valid;
	char opt_auto_move;
	time_t curr_time;
	time_t start_time;
	int timeout_sec = 10;
	int game_status;

	if (*game_in_progress == 0) {
		*game_in_progress = 1;
		generate_rand_cell(game);
		generate_rand_cell(game);
	}

	// draw game layout
	draw_game(window, game);
	start_time = time(NULL);
	while(1) {
		game_status = check_winner(game);
		if (game_status == 1) {
			break;
		}
		else if (game_status == -1) {
			break;
		}
		curr_time = time(NULL);
   		nodelay(stdscr, TRUE);
		info_panel(*score, game_status);
		valid = 0;
		key = getch();
		// if there was no input for timeout_sec seconds, move automatically		
		if (difftime(curr_time, start_time) >= timeout_sec){
			opt_auto_move = auto_move(game);
			if (opt_auto_move == 'U')
				key = KEY_UP;
			else if (opt_auto_move == 'D')
				key = KEY_DOWN;
			else if (opt_auto_move == 'R')
				key = KEY_RIGHT;
			else if (opt_auto_move == 'L')
				key = KEY_LEFT;
		}
		if (key > 0) {
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
    nodelay(stdscr, FALSE);
	if (game_status != 0) {
		clear();
		draw_game(window, game);
		info_panel(*score, game_status);
	}
}

void reset_game(int *game_in_progress, int game[][5], int *score)
{
	int i, j;
	*game_in_progress = 0;
	*score = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game[i][j] = 0;
}

int main()
{
	WINDOW *window = initscr();
	int game[5][5]={0};
	int game_in_progress = 0, score =0;
	int option_count = 3, selected = 0;
	int key;
	// menu options
	char options[5][20] = {"New Game", "Resume", "Quit"};

	// check if colors are availabe
	if (!(has_colors() && start_color() == OK)) {
		endwin();
		printf("No colors available. Exit");
		return 0;
	}
	init();
	while(1) {
		draw_menu(window, options, option_count, selected);//, &game_in_progress, game);
		key = getch();
		if (key == KEY_DOWN && selected < 2)
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
				reset_game(&game_in_progress, game, &score);
				break;
			} else if (strcmp(options[selected], "New Game") == 0) {
				move(0, 0);
				reset_game(&game_in_progress, game, &score);
				start_game(window, &game_in_progress, game, &score);
				clear();
			} else if (strcmp(options[selected], "Resume") == 0) {
				if (game_in_progress == 1) {
					move(0, 0);
					start_game(window, &game_in_progress, game, &score);
					clear();
				}
			}
		}
		clear();
	}
	delwin(window);
	
	endwin();
	return 0;
}