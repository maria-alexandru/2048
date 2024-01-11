#ifndef STORAGE_H
#define STORAGE_H

#include <stdio.h>
#include "utils.h"

void save_game(app_info *app);

void upload_game(app_info *app);

void save_top_score(app_info *app);

void upload_top_score(app_info *app);

void load_size_game(app_info *app, int size);

void load_crt_tops(app_info *app);

#endif
