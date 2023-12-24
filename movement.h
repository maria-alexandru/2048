#ifndef MOVEMENT_H
#define MOVEMENT_H

#include <stdio.h>

char auto_move(int game[][5]);

int move_left(int game[][5], int *score);

int move_right(int game[][5], int *score);

int move_up(int game[][5], int *score);

int move_down(int game[][5], int *score);

#endif