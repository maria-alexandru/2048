#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <ncurses.h>

void copy_info(int game[][5], int game_copy[][5]);

int count_cells(int game[][5]);

int auto_move(int game[][5]);

int move_left(int game[][5], int *score);

int move_right(int game[][5], int *score);

int move_up(int game[][5], int *score);

int move_down(int game[][5], int *score);

#endif