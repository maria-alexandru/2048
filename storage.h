#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include "utils.h"

void save_game(app_info *app_info);

void upload_game(app_info *app_info);

void save_top_score(app_info *app_info);//void save_top_score(game_stats *games, game_stats crt_game, int size);

void upload_top_score(app_info *app_info);

void load_size_game(app_info *app_info, int size);

void load_crt_tops(app_info *app_info);//void load_crt_tops(game_stats games[], game_stats *crt_game, int size);

#endif