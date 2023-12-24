#include "draw.h"
#include "utils.h"

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

