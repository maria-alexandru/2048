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

void start_game(WINDOW *window, app_info *app);
void reset_game(app_info *app);

int init(void)
{
	// check if colors are availabe
	if (!(has_colors() && start_color() == OK)) {
		endwin();
		printf("No colors available. Exit");
		return -1;
	}

	// no output when characters are typed
	noecho();
	// activate keyboard
	keypad(stdscr, TRUE);
	// no buffering
	cbreak();
	// hide cursor
	curs_set(0);
	srand(time(NULL));

	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
	refresh();

	// successfully initialized
	return 1;
}

/*
* Read user's input for number of seconds for auto move and print it on screen
*/
void enter_number(WINDOW *window, app_info *app)
{
	int c = '\0', i = 0;
	int max_x, max_y;
	char number[5] = "";
	//get size of the window
	getmaxyx(stdscr, max_y, max_x);

	while (c != '\n') {
		resize(&max_x, &max_y);
		draw_auto_move(app->auto_move_sec, 3);
		draw_screen_border(window);
		refresh();
		if ((c >= '0' && c <= '9') || c == KEY_BACKSPACE) {
			if (c == KEY_BACKSPACE) {
				if (i >= 1) {
					number[i - 1] = '\0';
					i--;
				}
			} else if (i < 2) {
				// maximum 2 digits
				if (!(i == 0 && c == '0')) {
					number[i++] = c;
					number[i] = '\0';
				}
			}
			number[i] = '\0';
			app->auto_move_sec = atoi(number);
			draw_auto_move(app->auto_move_sec, 3);
			draw_screen_border(window);
			refresh();
		}
		c = getch();
	}
}

/*
* Read user's input for player's name when a new top score is made and print it
* on screen
*/
void enter_name(WINDOW *window, app_info *app, char name[])
{
	int c = '\0', i = 0;
	int max_x, max_y;
	//get size of the window
	getmaxyx(stdscr, max_y, max_x);
	draw_hs_message(&app->crt_game, name, app->size);

	while (c != '\n') {
		if (resize(&max_x, &max_y) == 1) {
			draw_game(window, app->crt_game.game, app->size);
			draw_end_game(app->crt_game.status, app->size);
			draw_hs_message(&app->crt_game, name, app->size);
			draw_screen_border(window);
		}
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ||
			c == KEY_BACKSPACE) {
			if (c == KEY_BACKSPACE) {
				if (i >= 1) {
					name[i - 1] = '\0';
					i--;
				}
			} else if (i < 18) {
				// maximum 17 characters
				name[i++] = c;
				name[i] = '\0';
			}
			name[i] = '\0';
			draw_hs_message(&app->crt_game, name, app->size);
		}
		c = getch();
	}
}

/*
* Generate a random tile if the game matrix is not full
* The function searches all the empty tiles and then selects one
*/
void generate_rand_tile(int game[][MAX_DIM], int size)
{
	typedef struct {
		int i, j;
	} empty_tile;
	empty_tile empty_tiles[MAX_DIM * MAX_DIM];
	int empty = 0;
	int i, j;
	int rand_tile, rand_val;

	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (game[i][j] == 0) {
				empty_tiles[empty].i = i;
				empty_tiles[empty++].j = j;
			}
	if (empty != 0) {
		// generate a random index for the empty tile
		rand_tile = rand() % empty;
		// generate a random value
		rand_val = (rand() % 2 + 1) * 2;
		game[empty_tiles[rand_tile].i][empty_tiles[rand_tile].j] = rand_val;
	}
}

/*
* Check if 2048 value was reached
* @return 1 for win, -1 for loss, 0 for continue game
*/
int check_winner(int game[][MAX_DIM], int size)
{
	int i, j;
	int move = auto_move(game, size);

	// check loss
	if (move == '\0')
		return -1;

	// check win
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (game[i][j] == 2048)
				return 1;

	// continue game
	return 0;
}

/*
* Check if current score is a top score
* @return 1 for top score, 0 if it is not a top score
*/
int is_top_score(game_stats_info crt_game)
{
	int max = SCORES - 1;
	int i = SCORES - 1;
	while (i >= 0 &&
		   crt_game.top_scores[i].score != crt_game.score) {
		i--;
	}
	//check if the score is already in top scores list
	if (i >= 0 && crt_game.playing_time ==
				  crt_game.top_scores[i].time)
		return 0;

	// if current score is greater than the last top score, it is a top score
	if (crt_game.score > crt_game.top_scores[max].score)
		return 1;

	if (crt_game.score == crt_game.top_scores[max].score &&
		crt_game.playing_time < crt_game.top_scores[max].time)
		return 1;

	return 0;
}

/*
* Add current score to the top scores variable
*/
void add_top_score(game_stats_info *crt_game, char name[])
{
	int i;
	i = SCORES - 1;
	while (i >= 0 && crt_game->top_scores[i].score < crt_game->score) {
		crt_game->top_scores[i] = crt_game->top_scores[i - 1];
		i--;
	}
	i++;
	crt_game->top_scores[i].score = crt_game->score;
	crt_game->top_scores[i].time = crt_game->playing_time;
	crt_game->top_scores[i].status = crt_game->status;

	strcpy(crt_game->top_scores[i].player, name);
}

/*
* Check if current score is greater than the high score and update it
* @return 1 if high score has changed, 0 if not
*/
int update_high_score(game_stats_info *crt_game)
{
	if (crt_game->high_score < crt_game->score) {
		crt_game->high_score = crt_game->score;
		crt_game->high_score_time = crt_game->playing_time;
		return 1;
	} else if (crt_game->high_score == crt_game->score) {
		if (crt_game->playing_time < crt_game->high_score_time) {
			crt_game->high_score_time = crt_game->playing_time;
			return 1;
		}
	}
	return 0;
}

/*
* Print messages when the game ends and wait for pressing a key to continue
*/
void end_game(WINDOW *window, app_info *app)
{
	int key;
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int new_high_score = 0, new_top_score = 0;
	char name[20] = "";

	if (app->crt_game.status != 0) {
		clear();
		draw_game(window, app->crt_game.game, app->size);
		draw_end_game(app->crt_game.status, app->size);
		new_high_score = update_high_score(&app->crt_game);
		new_top_score = is_top_score(app->crt_game);
		if (new_high_score == 1 || new_top_score == 1) {
			enter_name(window, app, name);
			if (new_high_score == 1)
				strcpy(app->crt_game.player, name);
			if (new_top_score == 1)
				add_top_score(&app->crt_game, name);

			clear();
			draw_game(window, app->crt_game.game, app->size);
			draw_end_game(app->crt_game.status, app->size);
			new_high_score = 0;
			save_game(app);
			save_top_score(app);
		}
		nodelay(stdscr, TRUE);
		// wait until Q is pressed to exit
		while (1) {
			// terminal window was resized
			if (resize(&max_x, &max_y) == 1) {
				info_panel(*app);
				draw_game(window, app->crt_game.game, app->size);
				draw_end_game(app->crt_game.status, app->size);
			}
			info_panel(*app);
			draw_screen_border(window);
			key = getch();
			if (key == 'Q')
				break;
			else if (key == 'N')
				break;
		}
		nodelay(stdscr, FALSE);
	}
	save_game(app);
	if (key == 'N') {
		move(0, 0);
		reset_game(app);
		start_game(window, app);
	}
}

/*
* Execute a command during the game (moves and undo)
*/
void execute_commmand(int key, app_info *app, WINDOW *window)
{
	int game_aux[MAX_DIM][MAX_DIM];
	int score_aux;
	// copy the game matrix and the score in an auxiliary variable
	if (key != 'U' && key >= KEY_DOWN && key <= KEY_RIGHT) {
		copy_info(app->crt_game.game, game_aux, app->size);
		score_aux = app->crt_game.score;
	}
	int valid = 0;
	if (key == KEY_DOWN) {
		valid = move_down(app->crt_game.game, &app->crt_game.score, app->size);
	} else if (key == KEY_UP) {
		valid = move_up(app->crt_game.game, &app->crt_game.score, app->size);
	} else if (key == KEY_LEFT) {
		valid = move_left(app->crt_game.game, &app->crt_game.score, app->size);
	} else if (key == KEY_RIGHT) {
		valid = move_right(app->crt_game.game, &app->crt_game.score, app->size);
	} else if (key == 'U') {
		// undo the last move
		copy_info(app->crt_game.old_game, app->crt_game.game, app->size);
		app->crt_game.score = app->crt_game.old_score;
		draw_game(window, app->crt_game.game, app->size);
	}
	// if the move was valid, copy the game matrix and the score from the
	// auxiliary variable
	if (key != 'U' && valid == 1) {
		copy_info(game_aux, app->crt_game.old_game, app->size);
		app->crt_game.old_score = score_aux;
	}

	if (key == KEY_UP || key == KEY_DOWN || key == KEY_RIGHT ||
		key == KEY_LEFT) {
		if (valid == 1)
			generate_rand_tile(app->crt_game.game, app->size);
		draw_game(window, app->crt_game.game, app->size);
	}
}

/*
* Create a new game matrix if a new game was started
* Read user's input for the game
* Check if the game is a win or a loss
* Move automatically after a period of time if no keys were pressed
* Print the info panel
*/
void start_game(WINDOW *window, app_info *app)
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int key;
	time_t start_time, crt_time;
	int timeout_sec = app->auto_move_sec;
	int playing_time_aux = app->crt_game.playing_time;
	int diff_time;

	// a new game was started
	if (app->crt_game.in_progress == 0) {
		app->crt_game.in_progress = 1;
		generate_rand_tile(app->crt_game.game, app->size);
		generate_rand_tile(app->crt_game.game, app->size);
		copy_info(app->crt_game.game, app->crt_game.old_game, app->size);
		app->crt_game.old_score = app->crt_game.score;
	}

	// draw game layout
	draw_game(window, app->crt_game.game, app->size);
	start_time = time(NULL);
	nodelay(stdscr, TRUE);
	while (1) {
		if (resize(&max_x, &max_y) == 1) {
			// terminal window was resized
			info_panel(*app);
			draw_game(window, app->crt_game.game, app->size);
		}

		app->crt_game.status = check_winner(app->crt_game.game, app->size);
		if (app->crt_game.status == 1 ||
			app->crt_game.status == -1)
			break;

		crt_time = time(NULL);
		diff_time = (int)difftime(crt_time, start_time);
		app->crt_game.playing_time = playing_time_aux + diff_time;
		info_panel(*app);
		draw_screen_border(window);
		key = getch();

		// if there was no input for timeout_sec seconds, move automatically
		if (timeout_sec != -1 && difftime(crt_time, start_time) >= timeout_sec)
			key = auto_move(app->crt_game.game, app->size);
		if (key > 0) {
			playing_time_aux = app->crt_game.playing_time;
			start_time = time(NULL);
			if (key == 'Q')
				break;
			execute_commmand(key, app, window);
			save_game(app);
		}
	}
	nodelay(stdscr, FALSE);
	if (app->crt_game.status != 0)
		end_game(window, app);
}

/*
* Change everything to 0 when a new game is started
*/
void reset_game(app_info *app)
{
	int i, j;
	app->crt_game.in_progress = 0;
	app->crt_game.playing_time = 0;
	app->crt_game.score = 0;
	for (i = 0; i < app->size; i++)
		for (j = 0; j < app->size; j++)
			app->crt_game.game[i][j] = 0;
}

/*
* Open theme menu where user can select the desired theme
*/
void open_theme_menu(WINDOW *window, theme_info themes[], int theme_count,
					 int *theme_id)
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

/*
* Open the top scores page where user can see the last 8 top scores
*/
void open_top_scores(WINDOW *window, top_score top_scores[])
{
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	draw_top_scores(window, top_scores);

	while (1) {
		if (resize(&max_x, &max_y) == 1) {
			clear();
			draw_top_scores(window, top_scores);
			draw_screen_border(window);
			refresh();
		}
		int key = getch();
		if (key == 'Q' || key == '\n')
			break;
	}
}

/*
* Open the auto move page where user can change the option
*/
void open_auto_move(WINDOW *window, app_info *app)
{
	int selected, key;
	int max_x, max_y;
	getmaxyx(window, max_y, max_x);
	// back button
	selected = -1;
	while (1) {
		resize(&max_x, &max_y);
		draw_auto_move(app->auto_move_sec, selected);
		draw_screen_border(window);
		refresh();
		key = getch();
		if (key == 'Q')
			break;
		if (key == KEY_RIGHT && selected < 2)
			selected++;
		if (key == KEY_LEFT && selected > -1)
			selected--;
		if (key == KEY_UP && selected > -1)
			selected--;
		if (key == KEY_DOWN && selected < 2)
			selected++;
		if (key == '\n') {
			// back
			if (selected == -1)
				break;
			// turn auto move off
			if (selected == 0) {
				app->auto_move_sec = -1;
				break;
			} else if (selected == 1) {
				selected = 2;
			}
			if (selected == 2) {
				selected = 3;
				enter_number(window, app);
				selected = 2;
				break;
			}
		}
		refresh();
	}
	save_game(app);
}

/*
* Open settings menu where user can access the theme page or the auto move page
*/
void open_settings(WINDOW *window, app_info *app)
{
	clear();
	int max_x, max_y;
	int key, selected = 0;
	getmaxyx(stdscr, max_y, max_x);
	draw_screen_border(window);
	menu_info settings_menu;
	settings_menu.option_count = 3;
	strcpy(settings_menu.options[0], "Theme");
	strcpy(settings_menu.options[1], "Auto move");
	strcpy(settings_menu.options[2], "Back");

	while (1) {
		resize(&max_x, &max_y);
		draw_menu(window, settings_menu, selected);

		key = getch();
		if (key == KEY_DOWN && selected < settings_menu.option_count - 1)
			selected++;
		if (key == KEY_UP && selected > 0)
			selected--;
		if (key == 'Q')
			break;
		// enter is pressed
		if (key == '\n') {
			if (strcmp(settings_menu.options[selected], "Theme") == 0) {
				open_theme_menu(window, app->themes, app->theme_count,
								&app->theme_id);
				save_game(app);
			} else if (!strcmp(settings_menu.options[selected], "Auto move")) {
				open_auto_move(window, app);
			} else if (strcmp(settings_menu.options[selected], "Back") == 0) {
				break;
			}
		}
		clear();
	}
}

/*
* Open main menu where user can start a new game, resume the last game, access
* the top scores page, enter the settings menu or quit the game
*/
void open_main_menu(WINDOW *window, app_info *app, menu_info main_menu,
					menu_info size_menu)
{
	int key;
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	int selected = 0;
	int selected_size = size_menu.selected;

	while (1) {
		// refresh if terminal window size changes
		resize(&max_x, &max_y);
		draw_menu(window, main_menu, selected);
		key = getch();
		if (key == KEY_DOWN && selected < main_menu.option_count - 1)
			selected++;
		if (key == KEY_UP && selected > 0)
			selected--;
		if (strcmp(main_menu.options[selected], "Resume") == 0) {
			if (app->crt_game.in_progress == 0) {
				if (key == KEY_DOWN)
					selected++;
				else if (key == KEY_UP)
					selected--;
			}
		}
		if (selected == 0) {
			if (key == KEY_LEFT && selected_size > 0) {
				size_menu.selected--;
				selected_size--;
				strcpy(main_menu.options[0], size_menu.options[selected_size]);
				load_size_game(app, size_menu.selected + 3);
				load_crt_tops(app);
			}
			if (key == KEY_RIGHT &&
				selected_size < size_menu.option_count - 1) {
				size_menu.selected++;
				selected_size++;
				strcpy(main_menu.options[0], size_menu.options[selected_size]);
				load_size_game(app, size_menu.selected + 3);
				load_crt_tops(app);
			}
		}
		// enter is pressed
		if (key == '\n') {
			if (strcmp(main_menu.options[selected], "Quit") == 0) {
				clear();
				break;
			} else if (strcmp(main_menu.options[selected], "New Game") == 0) {
				move(0, 0);
				reset_game(app);
				start_game(window, app);
			} else if (strcmp(main_menu.options[selected], "Resume") == 0) {
				if (app->crt_game.in_progress == 1) {
					move(0, 0);
					app->crt_game.in_progress = 1;
					start_game(window, app);
				}
			} else if (strcmp(main_menu.options[selected], "Top Scores") == 0) {
				open_top_scores(window, app->crt_game.top_scores);
			} else if (strcmp(main_menu.options[selected], "Settings") == 0) {
				open_settings(window, app);
			}
		}
		clear();
	}
}

/*
* Draw the start page which contains the 2048 logo
*/
void start_page(WINDOW *window)
{
	int key;
	int max_x, max_y;
	getmaxyx(stdscr, max_y, max_x);
	draw_logo(window);
	nodelay(window, TRUE);
	while (1) {
		if (resize(&max_x, &max_y) == 1)
			draw_logo(window);
		key = getch();
		if (key == '\n')
			break;
	}
	nodelay(window, FALSE);
	clear();
}

int main(void)
{
	WINDOW *window = initscr();

	if (init() == -1)
		return 0;

	app_info app;
	menu_info main_menu;
	menu_info size_menu;

	size_menu.option_count = 3;
	strcpy(size_menu.options[0], "Small (3x3) >");
	strcpy(size_menu.options[1], "< Classic (4x4) >");
	strcpy(size_menu.options[2], "< Big (5x5) >");

	main_menu.option_count = 6;
	strcpy(main_menu.options[1], "New Game");
	strcpy(main_menu.options[2], "Resume");
	strcpy(main_menu.options[3], "Top Scores");
	strcpy(main_menu.options[4], "Settings");
	strcpy(main_menu.options[5], "Quit");

	upload_game(&app);
	size_menu.selected = app.size - 3;
	strcpy(main_menu.options[0], size_menu.options[size_menu.selected]);
	copy_info(app.crt_game.game, app.crt_game.old_game, app.size);
	app.crt_game.old_score = app.crt_game.score;
	upload_top_score(&app);
	app.theme_count = read_themes(app.themes);
	set_theme(app.themes[app.theme_id]);

	start_page(window);
	open_main_menu(window, &app, main_menu, size_menu);
	save_game(&app);
	save_top_score(&app);

	delwin(window);
	endwin();
	return 0;
}
