#include "movement.h"

/*
* Copy values from game matrix to game_copy matrix
*/
void copy_info(int game[][MAX_DIM], int game_copy[][MAX_DIM], int size)
{
	int i, j;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			game_copy[i][j] = game[i][j];
}

/*
* Count total number of occupied tiles in a given game matrix and return it
*/
int count_tiles(int game[][MAX_DIM], int size)
{
	int i, j;
	int total = 0;
	for (i = 0; i < size; i++)
		for (j = 0; j < size; j++)
			if (game[i][j] != 0)
				total++;
	return total;
}

/*
* Check all possible moves and choose the one that empties the maximum number of
* tiles
* @return The choosen direction key or 0 if there are no more valid moves
*/
int auto_move(int game[][MAX_DIM], int size)
{
	int game_copy[MAX_DIM][MAX_DIM];
	int i, j;
	int score_copy = 0;
	int total_tile_min = size * size, total_tile = 0;
	int operation = '\0';
	int valid = 0;

	copy_info(game, game_copy, size);
	valid = move_down(game_copy, &score_copy, size);
	total_tile = count_tiles(game_copy, size);
	if (valid == 1 && total_tile <= total_tile_min) {
		total_tile_min = total_tile;
		operation = KEY_DOWN;
	}

	copy_info(game, game_copy, size);
	valid = move_up(game_copy, &score_copy, size);
	total_tile = count_tiles(game_copy, size);
	if (valid == 1 && total_tile <= total_tile_min) {
		total_tile_min = total_tile;
		operation = KEY_UP;
	}

	copy_info(game, game_copy, size);
	valid = move_right(game_copy, &score_copy, size);
	total_tile = count_tiles(game_copy, size);
	if (valid == 1 && total_tile <= total_tile_min) {
		total_tile_min = total_tile;
		operation = KEY_RIGHT;
	}

	copy_info(game, game_copy, size);
	valid = move_left(game_copy, &score_copy, size);
	total_tile = count_tiles(game_copy, size);
	if (valid == 1 && total_tile <= total_tile_min) {
		total_tile_min = total_tile;
		operation = KEY_LEFT;
	}

	return operation;
}

/*
* Search tiles that are not 0 on each row
* If only a tile is found, move it on the first position in the row
* If more than one tile is found, combine the first two values in the row,
* then the next two if they exist and then move all tiles to
* the left
* @return 1 if the move is valid, 0 if not
*/
int move_left(int game[][MAX_DIM], int *score, int size)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (i = 0; i < size; i++) {
		k = 0;
		for (j = 0; j < size; j++) {
			// search a tile that is not 0
			while (j < size && game[i][j] == 0)
				j++;
			p1 = j;
			// if a tile was found
			if (j < size) {
				j++;
				// search another tile that is not 0
				while (j < size && game[i][j] == 0)
					j++;
				// another tile was found
				if (j < size) {
					// if the values are equal, add them into one tile
					if (game[i][j] == game[i][p1]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][j] = 0;
						game[i][p1] = 0;
						game[i][k] = aux1 + aux2;
						*score += game[i][k];
					} else {
						// if the values are not equal, move the tiles
						if (p1 != k || j != k + 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][p1] = 0;
						game[i][j] = 0;
						game[i][k] = aux2;
						game[i][k + 1] = aux1;
					}
					k++;
					j = k - 1;
				} else {
					// just a tile on the row
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

/*
* Search tiles that are not 0 on each row
* If only a tile is found, move it on the last position in the row
* If more than one tile is found, combine the last two values in the row,
* then the next two if they exist and then move all tiles to
* the right
* @return 1 if the move is valid, 0 if not
*/
int move_right(int game[][MAX_DIM], int *score, int size)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (i = 0; i < size; i++) {
		k = size - 1;
		for (j = size - 1; j >= 0; j--) {
			// search a tile that is not 0
			while (j >= 0 && game[i][j] == 0)
				j--;
			p1 = j;
			// if a tile was found
			if (j >= 0) {
				j--;
				// search another tile that is not 0
				while (j >= 0 && game[i][j] == 0)
					j--;
				// another tile was found
				if (j >= 0) {
					// if the values are equal, add them into one tile
					if (game[i][j] == game[i][p1]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][j] = 0;
						game[i][p1] = 0;
						game[i][k] = aux1 + aux2;
						*score += game[i][k];
					} else {
						// if the values are not equal, move the tiles
						if (p1 != k || j != k - 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[i][p1];
						game[i][p1] = 0;
						game[i][j] = 0;
						game[i][k] = aux2;
						game[i][k - 1] = aux1;
					}
					k--;
					j = k + 1;
				} else {
					// just a tile on the row
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

/*
* Search tiles that are not 0 on each column
* If only a tile is found, move it on the first position in the column
* If more than one tile is found, combine the first two values on the column,
* then the next two if they exist and then move all tiles up
* @return 1 if the move is valid, 0 if not
*/
int move_up(int game[][MAX_DIM], int *score, int size)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (j = 0; j < size; j++) {
		k = 0;
		for (i = 0; i < size; i++) {
			// search a tile that is not 0
			while (i < size && game[i][j] == 0)
				i++;
			p1 = i;
			// if a tile was found
			if (i < size) {
				i++;
				// search another tile that is not 0
				while (i < size && game[i][j] == 0)
					i++;
				// another tile was found
				if (i < size) {
					// if the values are equal, add them into one tile
					if (game[i][j] == game[p1][j]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[i][j] = 0;
						game[p1][j] = 0;
						game[k][j] = aux1 + aux2;
						*score += game[k][j];
					} else {
						// if the values are not equal, move the tiles
						if (p1 != k || i != k + 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[p1][j] = 0;
						game[i][j] = 0;
						game[k][j] = aux2;
						game[k + 1][j] = aux1;
					}
					k++;
					i = k - 1;
				} else {
					// just a tile on the column
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

/*
* Search tiles that are not 0 on each column
* If only a tile is found, move it on the last position in the column
* If more than one tile is found, combine the last two values on the column,
* then the next two if they exist and then move all tiles down
* @return 1 if the move is valid, 0 if not
*/
int move_down(int game[][MAX_DIM], int *score, int size)
{
	int i, j, k, p1, aux1, aux2;
	int valid = 0;

	for (j = 0; j < size; j++) {
		k = size - 1;
		for (i = size - 1; i >= 0; i--) {
			// search a tile that is not 0
			while (i >= 0 && game[i][j] == 0)
				i--;
			p1 = i;
			// if a tile was found
			if (i >= 0) {
				i--;
				// search another tile that is not 0
				while (i >= 0 && game[i][j] == 0)
					i--;
				// another tile was found
				if (i >= 0) {
					// if the values are equal, add them into one tile
					if (game[i][j] == game[p1][j]) {
						valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[i][j] = 0;
						game[p1][j] = 0;
						game[k][j] = aux1 + aux2;
						*score += game[k][j];
					} else {
						// if the values are not equal, move the tiles
						if (p1 != k || i != k - 1)
							valid = 1;
						aux1 = game[i][j];
						aux2 = game[p1][j];
						game[p1][j] = 0;
						game[i][j] = 0;
						game[k][j] = aux2;
						game[k - 1][j] = aux1;
					}
					k--;
					i = k + 1;
				} else {
					// just a tile on the column
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
