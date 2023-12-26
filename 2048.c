#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "utils.h"
#include "movement.h"
#include "draw.h"
#include "theme.h"
#include "storage.h"


void start_game(WINDOW *window, game_stats *game_stats);
void reset_game(game_stats *game_stats);

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
	
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	refresh();
	return 1; // successfully initialized
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
		return -1; 	// loss

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game[i][j] == 2048)
				return 1; // win

	// continue game
	return 0;
}

// return 1 if high score has changed
int update_high_score(game_stats *game_stats)
{
	if (game_stats->high_score < game_stats->score) {
		game_stats->high_score = game_stats->score;
		game_stats->high_score_time = game_stats->playing_time;
		return 1;
	} else if (game_stats->high_score == game_stats->score) {
		if (game_stats->playing_time < game_stats->high_score_time){
			game_stats->high_score_time = game_stats->playing_time;
			return 1;
		}
	}
	return 0;
}

void end_game(WINDOW *window, game_stats *game_stats)
{
	int key;
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int new_high_score = 0;
	if (game_stats->game_status != 0) {
		clear();
		draw_game(window, game_stats->game);
		draw_end_game(game_stats->game_status);
		new_high_score = update_high_score(game_stats);
		if (new_high_score == 1) {
			introduce_name(window, game_stats);//draw_hs_message(game_stats);
			clear();
			draw_game(window, game_stats->game);
			draw_end_game(game_stats->game_status);
			new_high_score = 0;
			save_game(*game_stats);
		}
		// wait until Q is pressed to exit
		while (1) {
			if (resize(&max_x, &max_y) == 1) { // terminal window was resized
				draw_game(window, game_stats->game);
				draw_end_game(game_stats->game_status);
			}
			info_panel(*game_stats, game_stats->game_status);
			key = getch();
			if (key == 'Q')
				break;
			else if (key == 'N')
				break;
		}
	}
	save_game(*game_stats);
	if (key == 'N') {
		move(0, 0);
		reset_game(game_stats);
		start_game(window, game_stats);
	}
}

void execute_commmand(int key, game_stats *game_stats, WINDOW *window)
{
	// copy the game matrix and the score
	if(key != 'U') {
		copy_info(game_stats->game, game_stats->old_game);
		game_stats->old_score = game_stats->score;
	}
	int valid = 0;
	if (key == KEY_DOWN)
		valid = move_down(game_stats->game, &game_stats->score);
	else if (key == KEY_UP)
		valid = move_up(game_stats->game, &game_stats->score);
	else if (key == KEY_LEFT)
		valid = move_left(game_stats->game, &game_stats->score);
	else if (key == KEY_RIGHT)
		valid = move_right(game_stats->game, &game_stats->score);
	else if (key == 'U'){
		// undo the last move
		copy_info(game_stats->old_game, game_stats->game);
		game_stats->score = game_stats->old_score;
		draw_game(window, game_stats->game);
	}

	if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT ||
		key == KEY_LEFT) {
		if (valid == 1)
			generate_rand_cell(game_stats->game);
		draw_game(window, game_stats->game);
	}
}

void start_game(WINDOW *window, game_stats *game_stats)
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int key;
	time_t start_time, curr_time;
	int timeout_sec = 10;
	int playing_time_aux = game_stats->playing_time;

	// a new game was started
	if (game_stats->game_in_progress == 0) {
		game_stats->game_in_progress = 1;
		generate_rand_cell(game_stats->game);
		generate_rand_cell(game_stats->game);	
		copy_info(game_stats->game, game_stats->old_game);
		game_stats->old_score = game_stats->score;
	}

	// draw game layout
	draw_game(window, game_stats->game);
	start_time = time(NULL);
	nodelay(stdscr, TRUE);
	while(1) {
		if (resize(&max_x, &max_y) == 1) // terminal window was resized
			draw_game(window, game_stats->game);

		game_stats->game_status = check_winner(game_stats->game);
		if (game_stats->game_status == 1 || game_stats->game_status == -1)
			break;

		curr_time = time(NULL);
		game_stats->playing_time = playing_time_aux + (int)difftime(curr_time, start_time);
		info_panel(*game_stats, game_stats->game_status);
		key = getch();

		// if there was no input for timeout_sec seconds, move automatically		
		if (difftime(curr_time, start_time) >= timeout_sec) {
			key = auto_move(game_stats->game);
			save_game(*game_stats); // save the game
		}
		if (key > 0) {
			playing_time_aux = game_stats->playing_time;
			start_time = time(NULL);
			if (key == 'Q')
				break;
			else
				execute_commmand(key, game_stats, window);
		}
	}
    nodelay(stdscr, FALSE);
	if (game_stats->game_status != 0)
		end_game(window, game_stats);
}

void reset_game(game_stats *game_stats)
{
	int i, j;
	game_stats->game_in_progress = 0;
	game_stats->playing_time = 0;
	game_stats->score = 0;
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game_stats->game[i][j] = 0;
}

void open_theme_menu(WINDOW *window, theme themes[], int theme_count, int *theme_id)
{
	int cursor_pos_x = 0, cursor_pos_y = 0;
	int max_x, max_y;
	int key, selected = *theme_id;

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
			*theme_id = selected;
			set_theme(themes[selected]);
			break;
		}
	}
}

void open_main_menu(WINDOW *window, game_stats *game_stats, menu main_menu, 
					theme themes[], int theme_count)
{
	int key;
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int selected = 0;

	while(1) {
		resize(&max_x, &max_y); // refresh if terminal window size changes
		draw_menu(window, main_menu, selected);
		key = getch();
		if (key == KEY_DOWN && selected < main_menu.option_count - 1)
			selected++;
		if (key == KEY_UP && selected > 0)
			selected--;
		if (strcmp(main_menu.options[selected], "Resume") == 0) {
			if (game_stats->game_in_progress == 0) {
				if (key == KEY_DOWN)
					selected++;
				else if (key == KEY_UP)
					selected--;
			}
		}
		// enter is pressed
		if (key == '\n') {
			if (strcmp(main_menu.options[selected], "Quit") == 0) {
				clear();
				break;
			} else if (strcmp(main_menu.options[selected], "New Game") == 0) {
				move(0, 0);
				reset_game(game_stats);
				start_game(window, game_stats);
				clear();
			} else if (strcmp(main_menu.options[selected], "Resume") == 0) {
					if (game_stats->game_in_progress == 1) {
						move(0, 0);
						game_stats->game_in_progress = 1;
						start_game(window, game_stats);
						clear();
					}
			} else if (strcmp(main_menu.options[selected], "Theme") == 0) {
				open_theme_menu(window, themes, theme_count, &game_stats->theme_id);
				save_game(*game_stats);
				clear();
			}
		}
		clear();
	}
}

int main()
{
	WINDOW *window = initscr();
	if (init() == -1)
		return 0;

	game_stats game_stats;
	menu main_menu;
	theme themes[10];
	int theme_count = 0;

	main_menu.option_count = 4;
	strcpy(main_menu.options[0], "New Game");
	strcpy(main_menu.options[1], "Resume");
	strcpy(main_menu.options[2], "Theme");
	strcpy(main_menu.options[3], "Quit");

	upload_game(&game_stats);
	copy_info(game_stats.game, game_stats.old_game);
	game_stats.old_score = game_stats.score;
	theme_count = read_themes(themes);
	set_theme(themes[game_stats.theme_id]);	
	open_main_menu(window, &game_stats, main_menu, themes, theme_count);
	save_game(game_stats);

	delwin(window);
	endwin();
	return 0;
}