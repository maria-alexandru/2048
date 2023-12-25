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
    f = fopen(GAME_FILE, "r");
	if (f == NULL)
		printf("Game file not found!");
    else {
        fscanf(f, "%d", &game_stats->theme_id);
        fscanf(f, "%d %d", &game_stats->high_score, &game_stats->high_score_time);
        fgets(game_stats->player, 50, f);
        fgets(game_stats->player, 50, f);
        game_stats->player[strlen(game_stats->player) - 1] = '\0'; // remove \n
        fscanf(f, "%d", &game_stats->playing_time);
        fscanf(f, "%d", &game_stats->score);
        for (i = 0; i < 4; i++) {
            for (j = 0; j < 4; j++)
                fscanf(f, "%d ", &game_stats->game[i][j]);
        }
        fclose(f);
    }
}