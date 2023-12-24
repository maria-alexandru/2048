#include <stdio.h>
#include <curses.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <sys/select.h>

void start_game(WINDOW *window, int *game_in_progress, int game[][5], int *score);
void draw_menu(WINDOW *window, char options[][20], int option_count, int selected);//draw_menu(WINDOW *window, int *game_in_progress, int game[][5]);
void draw_game(WINDOW *window, int game[][5]);
void int_to_string(char *s, int val, int format);
void rectangle(int x, int y, int size_x, int size_y);
char auto_move(int game[][5]);

void draw_screen_border(WINDOW *window)
{
	attron(COLOR_PAIR(2));
	attron(A_STANDOUT);
	// menu border
	box(window, 0, 0);
	attroff(A_STANDOUT);
	attroff(COLOR_PAIR(2));
	wrefresh(window);
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
}

void info_panel(int score, int status)
{
	char date[100], time_now[100], score_text[100];
	struct tm *time_p;
	int x, y, size_x, size_y;
	int key;

	getmaxyx(stdscr, size_y, size_x);
	size_x = 20;
	size_y -= 4;
	x = 2;
	y = 2;

	// get time
	time_t t = time(NULL);
	time_p = localtime(&t);
	timestr(*time_p, time_now);
	datestr(*time_p, date);
	int_to_string(score_text, score, 2);
	
	attron(COLOR_PAIR(3));
	// print date, time and score
	rectangle(1, 1, size_x, size_y);
	mvaddstr(y, x, date);
	mvaddstr(y + 1, x, time_now);
	mvaddstr(y + 3, x, "Score: ");
	mvaddstr(y + 3, x + strlen("Score: "), score_text);
	y += 5;

	if (status == 0)
		print_valid_input(x, y); // if game is not over, print valid commands
	else if (status == -1)
		mvaddstr(y, x, "Game over!");
	else if (status == 1)
		mvaddstr(y, x, "Win!");
	if (status == 1 || status == -1) {
		y++;
		// if game is over, wait until Q is pressed
		while (1) {
			mvaddstr(y, x, "Press Q to exit");
			key = getch();
			if (key == 'Q')
				break;
		}
	}
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
	move(cursor_pos_y, cursor_pos_x);

	// create colors
	init_color(20, 871, 507, 421); // red
	init_color(21, 968, 996, 820); // yellow
	init_color(22, 996, 796, 796); // pink
	init_color(23, 441, 185, 687); // purple
	init_color(24, 554, 652, 910); // dark blue
	init_color(25, 539, 851, 695); // blue
	init_color(26, 812, 945, 820); // green
	init_color(27, 859, 746, 996); // light purple
	init_color(28, 0, 503, 437);   // dark green
	init_color(29, 976, 375, 554); // pink
	init_color(30, 996, 593, 0);   //orange
	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	for (i = 20; i <= 30; i++)
		init_pair(i, COLOR_BLUE, i);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			x = cursor_pos_x + j * cell_size * 2;
			y = cursor_pos_y + i * cell_size;
			//draw cell
			attron(COLOR_PAIR(3));
			rectangle(x, y, 2 * cell_size, cell_size);
			//print cell value`
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
	attron(COLOR_PAIR(3));
	rectangle(cursor_pos_x, cursor_pos_y, 2 * layout_size, layout_size);
	draw_screen_border(window);
}

int move_left(int game[][5], int *score)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (i = 0; i < 4; i++) {
		k = 0;
		for (j = 0; j < 4; j++) {
			// search a cell that is not 0
			while (j < 4 && game[i][j] == 0)
				j++;
			p1 = j;
			// if a cell was found
			if (j < 4) {
				j++;
				// search another cell that is not 0
				while (j < 4 && game[i][j] == 0)
					j++;
				// another cell was found
				if (j < 4) {
					// if the values are equal, add them into one cell
					if (game[i][j] == game[i][p1]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][j] = 0;
						game[i][p1] = 0;
						game[i][k] = aux1 + aux2;
						*score += game[i][k];
					} else{
						// if the values are not equal, move the cells
						if (p1 != k || j != k + 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][p1] = 0;
						game[i][j] = 0;		
						game[i][k] = aux2;
						game[i][k+1] = aux1;						
					}
					k++;
					j = k-1;
				} else {
					// just a cell on the line
					if (p1 != k)
						valid = 1;
					aux1 = game[i][p1];
					game[i][p1] = 0;
					game[i][k] = aux1;
				}
			}
		}
	}
	return valid;
}

int move_right(int game[][5], int *score)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (i = 0; i < 4; i++) {
		k = 3;
		for (j = 3; j >= 0; j--) {
			// search a cell that is not 0
			while (j >= 0 && game[i][j] == 0)
				j--;
			p1 = j;
			// if a cell was found
			if (j >= 0) {
				j--;
				// search another cell that is not 0
				while (j >= 0 && game[i][j] == 0)
					j--;
				// another cell was found
				if (j >= 0) {
					// if the values are equal, add them into one cell
					if (game[i][j] == game[i][p1]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][j] = 0;
						game[i][p1] = 0;
						game[i][k] = aux1 + aux2;
						*score += game[i][k];
					} else {
						// if the values are not equal, move the cells
						if (p1 != k || j != k - 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][p1] = 0;
						game[i][j] = 0;		
						game[i][k] = aux2;
						game[i][k-1] = aux1;						
					}
					k--;
					j = k+1;
				} else {
					// just a cell on the line
					if (p1 != k)
						valid = 1;
					aux1 = game[i][p1];
					game[i][p1] = 0;
					game[i][k] = aux1;
				}
			}
		}
	}
	return valid;
}

int move_up(int game[][5], int *score)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (j = 0; j < 4; j++) {
		k = 0;
		for (i = 0; i < 4; i++) {
			// search a cell that is not 0
			while (i < 4 && game[i][j] == 0)
				i++;
			p1 = i;
			// if a cell was found
			if (i < 4) {
				i++;
				// search another cell that is not 0
				while (i < 4 && game[i][j] == 0)
					i++;
				// another cell was found
				if (i < 4) {
					// if the values are equal, add them into one cell
					if (game[i][j] == game[p1][j]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[i][j] = 0;
						game[p1][j] = 0;
						game[k][j] = aux1 + aux2;
						*score += game[k][j];
					} else {
						// if the values are not equal, move the cells
						if (p1 != k || i != k + 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[p1][j] = 0;
						game[i][j] = 0;		
						game[k][j] = aux2;
						game[k+1][j] = aux1;				
					}
					k++;
					i = k-1;
				} else {
					// just a cell on the column
					if (p1 != k)
						valid = 1;
					aux1 = game[p1][j];
					game[p1][j] = 0;
					game[k][j] = aux1;
				}
			}
		}
	}
	return valid;
}

int move_down(int game[][5], int *score)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (j = 0; j < 4; j++) {
		k = 3;
		for (i = 3; i >= 0; i--) {
			// search a cell that is not 0
			while (i >= 0 && game[i][j] == 0)
				i--;
			p1 = i;
			// if a cell was found
			if (i >= 0) {
				i--;
				// search another cell that is not 0
				while (i >= 0 && game[i][j] == 0)
					i--;
				// another cell was found
				if (i >= 0) {
					// if the values are equal, add them into one cell
					if (game[i][j] == game[p1][j]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[i][j] = 0;
						game[p1][j] = 0;
						game[k][j] = aux1 + aux2;
						*score += game[k][j];
					} else {
						// if the values are not equal, move the cells
						if (p1 != k || i != k - 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[p1][j] = 0;
						game[i][j] = 0;		
						game[k][j] = aux2;
						game[k-1][j] = aux1;					
					}
					k--;
					i = k+1;
				} else {
					// just a cell on the column
					if (p1 != k)
						valid = 1;
					aux1 = game[p1][j];
					game[p1][j] = 0;
					game[k][j] = aux1;
				}
			}
		}
	}
	return valid;
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

char auto_move(int game[][5])
{
	int game_copy[5][5];
	int i, j;
	int score_copy = 0;
	int total_cell_min = 16, total_cell = 0;
	char operation = '\0';
	int valid = 0;

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game_copy[i][j] = game[i][j];	
	valid = move_down(game_copy, &score_copy);
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game_copy[i][j] != 0)
				total_cell++;
	if (valid == 1 && total_cell <= total_cell_min) {
		total_cell_min = total_cell;
		operation = 'D';
	}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game_copy[i][j] = game[i][j];
	total_cell = 0;
	valid = move_up(game_copy, &score_copy);
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game_copy[i][j] != 0)
				total_cell++;
	if (valid == 1 && total_cell <= total_cell_min) {
		total_cell_min = total_cell;
		operation = 'U';
	}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game_copy[i][j] = game[i][j];
	total_cell = 0;
	valid = move_right(game_copy, &score_copy);
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game_copy[i][j] != 0)
				total_cell++;
	if (valid == 1 && total_cell <= total_cell_min) {
		total_cell_min = total_cell;
		operation = 'R';
	}

	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			game_copy[i][j] = game[i][j];
	total_cell = 0;
	valid = move_left(game_copy, &score_copy);
	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
			if (game_copy[i][j] != 0)
				total_cell++;
	if (valid == 1 && total_cell <= total_cell_min) {
		total_cell_min = total_cell;
		operation = 'L';
	}

	return operation;
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

void draw_menu(WINDOW *window, char options[][20], int option_count, int selected)
{
	int cursor_pos_x = 0, cursor_pos_y = 0;
	int max_x, max_y;
	int i;

	init_pair(3, COLOR_BLUE, COLOR_WHITE);
	init_pair(2, COLOR_BLUE, COLOR_BLUE);

	// get max size of the window
	getmaxyx(stdscr, max_y, max_x);
	// move the cursor to the center so that the text will be centered
	move(cursor_pos_y, cursor_pos_x);

	draw_screen_border(window);
	wrefresh(window);
	attron(COLOR_PAIR(3));
	cursor_pos_y = (max_y -  2 * option_count) / 2 ;
	cursor_pos_x = (max_x - 9) / 2;
	for (i = 0; i < option_count; i++) {
		// highlight option if it is selected
		if (selected == i)
			attron(A_STANDOUT);
		else
			attroff(A_STANDOUT);
		// display options
		mvaddstr(cursor_pos_y, cursor_pos_x, options[i]);
		cursor_pos_y += 2;
		move(cursor_pos_y, cursor_pos_x);
	}	
	wrefresh(window);
}

void init()
{
	// no output when characters are typed
	noecho();
	// activate keyboard
	keypad(stdscr, TRUE);
	// no buffering
	cbreak();
	// hide cursor	
	curs_set(0);
	srand(time(NULL));

	refresh();
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	bkgd(COLOR_PAIR(1));
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