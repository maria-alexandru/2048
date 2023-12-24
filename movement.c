#include "movement.h"

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