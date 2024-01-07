#include "storage.h"
#include "utils.h"
#define GAME_FILE "game.txt"
#define SCORE_FILE "scores.txt"

/*
* Save the game in the GAME_FILE file
*/
void save_game(app_info *app_info)
{
	int i, j, k;
	FILE *f;
	f = fopen(GAME_FILE, "w");

	if (f == NULL)
		printf("Game file not found!");
	else {
		fprintf(f, "%d\n", app_info->theme_id);
		fprintf(f, "%d\n", app_info->size);
		fprintf(f, "%d\n", app_info->auto_move_sec);

		k = app_info->size;
		app_info->games[k].high_score = app_info->crt_game.high_score;
		app_info->games[k].high_score_time = app_info->crt_game.high_score_time;
		strcpy(app_info->games[k].player, app_info->crt_game.player);
		app_info->games[k].playing_time = app_info->crt_game.playing_time;
		app_info->games[k].score = app_info->crt_game.score;
		app_info->games[k].game_in_progress = app_info->crt_game.game_in_progress;
		app_info->games[k].game_status = app_info->crt_game.game_status;
		for (i = 0; i < k; i++) {
			for (j = 0; j < k; j++)
				app_info->games[k].game[i][j] = app_info->crt_game.game[i][j];
		}

		for (k = 3; k < MAX_DIM; k++) {
			fprintf(f, "%d\n", app_info->games[k].high_score);
			fprintf(f, "%d\n", app_info->games[k].high_score_time);
			fprintf(f, "%s\n", app_info->games[k].player);
			fprintf(f, "%d\n", app_info->games[k].playing_time);
			fprintf(f, "%d\n", app_info->games[k].score);
			fprintf(f, "%d\n", app_info->games[k].game_in_progress);
			fprintf(f, "%d\n", app_info->games[k].game_status);
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					fprintf(f, "%d ", app_info->games[k].game[i][j]);
				fprintf(f, "\n");
			}
		}
		fclose(f);
	}
}

/*
* Upload the game from the GAME_FILE file if it exists or initialize the game
* and save it in the file
*/
void upload_game(app_info *app_info)
{
	int i, j, k;
	FILE *f;
	f = fopen(GAME_FILE, "rw");
	if (f == NULL) {
		// if file does not exist, create it
		f = fopen(GAME_FILE, "wr");
		// if file is empty, write it
		fclose(f);
		app_info->size = 4;
		app_info->theme_id = 0;
		app_info->auto_move_sec = 10;

		for (k = 3; k < MAX_DIM ; k++) {
			app_info->games[k].high_score = 0;
			app_info->games[k].high_score_time = 0;
			strcpy(app_info->games[k].player, "-");
			app_info->games[k].playing_time = 0;
			app_info->games[k].score = 0;
			app_info->games[k].game_in_progress = 0;
			app_info->games[k].game_status = 0;
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					app_info->games[k].game[i][j] = 0;
			}
		}
		load_size_game(app_info, app_info->size);
		save_game(app_info);
	} else {
		fscanf(f, "%d", &app_info->theme_id);
		fscanf(f, "%d", &app_info->size);
		fscanf(f, "%d", &app_info->auto_move_sec);

		for (k = 3; k < MAX_DIM; k++) {
			fscanf(f, "%d", &app_info->games[k].high_score);
			fscanf(f, "%d", &app_info->games[k].high_score_time);
			fscanf(f, "%s", app_info->games[k].player);
			fscanf(f, "%d", &app_info->games[k].playing_time);
			fscanf(f, "%d", &app_info->games[k].score);
			fscanf(f, "%d", &app_info->games[k].game_in_progress);
			fscanf(f, "%d", &app_info->games[k].game_status);
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					fscanf(f, "%d", &app_info->games[k].game[i][j]);
			}
		}
		load_size_game(app_info, app_info->size);
		fclose(f);
	}
}

/*
* Load current game with the desired size
*/
void load_size_game(app_info *app_info, int size)
{
	int i, j, k;

	k = size;
	app_info->size = k;
	app_info->crt_game.high_score = app_info->games[k].high_score;
	app_info->crt_game.high_score_time = app_info->games[k].high_score_time;
	strcpy(app_info->crt_game.player, app_info->games[k].player);
	app_info->crt_game.playing_time = app_info->games[k].playing_time;
	app_info->crt_game.score = app_info->games[k].score;
	app_info->crt_game.game_in_progress = app_info->games[k].game_in_progress;
	app_info->crt_game.game_status = app_info->games[k].game_status;
	for (i = 0; i < k; i++) {
		for (j = 0; j < k; j++)
			app_info->crt_game.game[i][j] = app_info->games[k].game[i][j];
	}
	save_game(app_info);
}

/*
* Load current top scores for the game with the desired size
*/
void load_crt_tops(app_info *app_info, int size)
{
	int i, k;
	k = size;
	for (i = 0; i < SCORES; i++) {
		app_info->crt_game.top_scores[i].score = app_info->games[k].top_scores[i].score;
		app_info->crt_game.top_scores[i].time = app_info->games[k].top_scores[i].time;
		app_info->crt_game.top_scores[i].game_status = app_info->games[k].top_scores[i].game_status;
		strcpy(app_info->crt_game.top_scores[i].player, app_info->games[k].top_scores[i].player);
	}
}

/*
* Save top scores in the SCORE_FILE file
*/
void save_top_score(app_info *app_info)
{
	int i, k;
	FILE *f;
	f = fopen(SCORE_FILE, "w");
	if (f != NULL) {
		k = app_info->size;
		for (i = 0; i < SCORES; i++) {
			app_info->games[k].top_scores[i].score = app_info->crt_game.top_scores[i].score;
			app_info->games[k].top_scores[i].time = app_info->crt_game.top_scores[i].time;
			app_info->games[k].top_scores[i].game_status = app_info->crt_game.top_scores[i].game_status;
			strcpy(app_info->games[k].top_scores[i].player, app_info->crt_game.top_scores[i].player);
		}

		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				fprintf(f, "%d\n", app_info->games[k].top_scores[i].score);
				fprintf(f, "%d\n", app_info->games[k].top_scores[i].time);
				fprintf(f, "%d\n", app_info->games[k].top_scores[i].game_status);
				fprintf(f, "%s\n", app_info->games[k].top_scores[i].player);
			}
		}
		fclose(f);
	}
}

/*
* Upload top scores from the SCORES_FILE file if it exists or initialize them
* with 0 and save in the file
*/
void upload_top_score(app_info *app_info)
{
	int i, k;
	FILE *f;

	f = fopen(SCORE_FILE, "rw");
	if (f == NULL) {
		// if file does not exist, create it
		f = fopen(SCORE_FILE, "wr");
		// if file is empty, write it
		fclose(f);
		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				app_info->games[k].top_scores[i].score = 0;
				app_info->games[k].top_scores[i].time = 0;
				app_info->games[k].top_scores[i].game_status = 0;
				strcpy(app_info->games[k].top_scores[i].player, "-");
			}
		}
		load_crt_tops(app_info, app_info->size);
		save_top_score(app_info);
	} else {
		fclose(f);
		f = fopen(SCORE_FILE, "r");
		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				fscanf(f, "%d", &app_info->games[k].top_scores[i].score);
				fscanf(f, "%d", &app_info->games[k].top_scores[i].time);
				fscanf(f, "%d", &app_info->games[k].top_scores[i].game_status);
				fscanf(f, "%s", app_info->games[k].top_scores[i].player);
			}
		}
		load_crt_tops(app_info, app_info->size);
		fclose(f);
	}
}
