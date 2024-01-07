#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>
#include <ncurses.h>
#include "utils.h"

void copy_info(int game[][MAX_DIM], int game_copy[][MAX_DIM], int size);

int count_tiles(int game[][MAX_DIM], int size);

int auto_move(int game[][MAX_DIM], int size);

int move_left(int game[][MAX_DIM], int *score, int size);

int move_right(int game[][MAX_DIM], int *score, int size);

int move_up(int game[][MAX_DIM], int *score, int size);

int move_down(int game[][MAX_DIM], int *score, int size);

#endif