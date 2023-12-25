#include "storage.h"
#include "utils.h"
#define GAME_FILE "game.txt"

void save_game(game_stats game_stats)
{
    int i, j;
    FILE *f;
    f = fopen(GAME_FILE, "w");
	if (f == NULL)
		printf("Game file not found!");
    else {
        fprintf(f, "%d\n", game_stats.theme_id);
        fprintf(f, "%d %d\n", game_stats.high_score, game_stats.high_score_time);
        fprintf(f, "%s\n", game_stats.player);
        fprintf(f, "%d\n", game_stats.playing_time);
        fprintf(f, "%d\n", game_stats.score);
        fprintf(f, "%d\n", game_stats.game_in_progress);
        fprintf(f, "%d\n", game_stats.game_status);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                fprintf(f, "%d ", game_stats.game[i][j]);
            fprintf(f, "\n");
        }
        fclose(f);
    }
}

void upload_game(game_stats *game_stats)
{
    int i, j;
    FILE *f;
    f = fopen(GAME_FILE, "rw");
	if (f == NULL) {
        // if file does not exist, create it
        f = fopen(GAME_FILE, "wr");
    }
    if (fscanf(f, "%d", &game_stats->theme_id) != EOF) {
        fscanf(f, "%d %d", &game_stats->high_score, &game_stats->high_score_time);
        fgets(game_stats->player, 20, f);
        fgets(game_stats->player, 20, f);
        game_stats->player[strlen(game_stats->player) - 1] = '\0'; // remove \n
        fscanf(f, "%d", &game_stats->playing_time);
        fscanf(f, "%d", &game_stats->score);
        fscanf(f, "%d", &game_stats->game_in_progress);
        fscanf(f, "%d", &game_stats->game_status);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                fscanf(f, "%d ", &game_stats->game[i][j]);  
        }
        fclose(f);
    } else {
        // if file is empty, write it
        fclose(f);
        game_stats->theme_id = 0;
        game_stats->high_score = 0;
        game_stats->high_score_time = 0;
        strcpy(game_stats->player, " ");
        game_stats->playing_time = 0;
        game_stats->score = 0;
        game_stats->game_in_progress = 0;
        game_stats->game_status = 0;
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                game_stats->game[i][j] = 0;
        }
        save_game(*game_stats);
    }
}