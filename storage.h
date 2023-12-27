#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include "utils.h"

void save_game(game_stats game_stats);

void upload_game(game_stats *game_stats);

void save_top_score(top_score top_scores[]);

void upload_top_score(top_score top_scores[]);

#endif