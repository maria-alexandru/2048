#include "storage.h"
#include "utils.h"
#define GAME_FILE "game.txt"
#define SCORE_FILE "scores.txt"

/*
* Save the game in the GAME_FILE file
*/
void save_game(app_info *app)
{
	int i, j, k;
	FILE *f;
	f = fopen(GAME_FILE, "w");

	if (!f) {
		printf("Game file not found!");
	} else {
		fprintf(f, "%d\n", app->theme_id);
		fprintf(f, "%d\n", app->size);
		fprintf(f, "%d\n", app->auto_move_sec);

		k = app->size;
		app->games[k].high_score = app->crt_game.high_score;
		app->games[k].high_score_time = app->crt_game.high_score_time;
		strcpy(app->games[k].player, app->crt_game.player);
		app->games[k].playing_time = app->crt_game.playing_time;
		app->games[k].score = app->crt_game.score;
		app->games[k].in_progress = app->crt_game.in_progress;
		app->games[k].status = app->crt_game.status;
		for (i = 0; i < k; i++) {
			for (j = 0; j < k; j++)
				app->games[k].game[i][j] = app->crt_game.game[i][j];
		}

		for (k = 3; k < MAX_DIM; k++) {
			fprintf(f, "%d\n", app->games[k].high_score);
			fprintf(f, "%d\n", app->games[k].high_score_time);
			fprintf(f, "%s\n", app->games[k].player);
			fprintf(f, "%d\n", app->games[k].playing_time);
			fprintf(f, "%d\n", app->games[k].score);
			fprintf(f, "%d\n", app->games[k].in_progress);
			fprintf(f, "%d\n", app->games[k].status);
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					fprintf(f, "%d ", app->games[k].game[i][j]);
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
void upload_game(app_info *app)
{
	int i, j, k;
	FILE *f;
	f = fopen(GAME_FILE, "rw");
	if (!f) {
		// if file does not exist, create it
		f = fopen(GAME_FILE, "wr");
		// if file is empty, write it
		fclose(f);
		app->size = 4;
		app->theme_id = 0;
		app->auto_move_sec = 10;

		for (k = 3; k < MAX_DIM ; k++) {
			app->games[k].high_score = 0;
			app->games[k].high_score_time = 0;
			strcpy(app->games[k].player, "-");
			app->games[k].playing_time = 0;
			app->games[k].score = 0;
			app->games[k].in_progress = 0;
			app->games[k].status = 0;
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					app->games[k].game[i][j] = 0;
			}
		}
		load_size_game(app, app->size);
		save_game(app);
	} else {
		fscanf(f, "%d", &app->theme_id);
		fscanf(f, "%d", &app->size);
		fscanf(f, "%d", &app->auto_move_sec);

		for (k = 3; k < MAX_DIM; k++) {
			fscanf(f, "%d", &app->games[k].high_score);
			fscanf(f, "%d", &app->games[k].high_score_time);
			fscanf(f, "%s", app->games[k].player);
			fscanf(f, "%d", &app->games[k].playing_time);
			fscanf(f, "%d", &app->games[k].score);
			fscanf(f, "%d", &app->games[k].in_progress);
			fscanf(f, "%d", &app->games[k].status);
			for (i = 0; i < k; i++) {
				for (j = 0; j < k; j++)
					fscanf(f, "%d", &app->games[k].game[i][j]);
			}
		}
		load_size_game(app, app->size);
		fclose(f);
	}
}

/*
* Load current game with the desired size
*/
void load_size_game(app_info *app, int size)
{
	int i, j, k;

	k = size;
	app->size = k;
	app->crt_game.high_score = app->games[k].high_score;
	app->crt_game.high_score_time = app->games[k].high_score_time;
	strcpy(app->crt_game.player, app->games[k].player);
	app->crt_game.playing_time = app->games[k].playing_time;
	app->crt_game.score = app->games[k].score;
	app->crt_game.in_progress = app->games[k].in_progress;
	app->crt_game.status = app->games[k].status;
	for (i = 0; i < k; i++) {
		for (j = 0; j < k; j++)
			app->crt_game.game[i][j] = app->games[k].game[i][j];
	}
	save_game(app);
}

/*
* Load current top scores for the game with the desired size
*/
void load_crt_tops(app_info *app)
{
	game_stats_info *games = app->games;
	game_stats_info *crt_game = &app->crt_game;
	int size = app->size;
	int i, k;
	k = size;
	for (i = 0; i < SCORES; i++) {
		crt_game->top_scores[i].score = games[k].top_scores[i].score;
		crt_game->top_scores[i].time = games[k].top_scores[i].time;
		crt_game->top_scores[i].status = games[k].top_scores[i].status;
		strcpy(crt_game->top_scores[i].player, games[k].top_scores[i].player);
	}
}

/*
* Save top scores in the SCORE_FILE file
*/
void save_top_score(app_info *app)
{
	game_stats_info *games = app->games;
	game_stats_info crt_game = app->crt_game;
	int size = app->size;
	int i, k;
	FILE *f;
	f = fopen(SCORE_FILE, "w");
	if (f) {
		k = size;
		for (i = 0; i < SCORES; i++) {
			games[k].top_scores[i].score = crt_game.top_scores[i].score;
			games[k].top_scores[i].time = crt_game.top_scores[i].time;
			games[k].top_scores[i].status = crt_game.top_scores[i].status;
			strcpy(games[k].top_scores[i].player,
				   crt_game.top_scores[i].player);
		}

		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				fprintf(f, "%d\n", games[k].top_scores[i].score);
				fprintf(f, "%d\n", games[k].top_scores[i].time);
				fprintf(f, "%d\n", games[k].top_scores[i].status);
				fprintf(f, "%s\n", games[k].top_scores[i].player);
			}
		}
		fclose(f);
	}
}

/*
* Upload top scores from the SCORES_FILE file if it exists or initialize them
* with 0 and save in the file
*/
void upload_top_score(app_info *app)
{
	int i, k;
	FILE *f;

	f = fopen(SCORE_FILE, "rw");
	if (!f) {
		// if file does not exist, create it
		f = fopen(SCORE_FILE, "wr");
		// if file is empty, write it
		fclose(f);
		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				app->games[k].top_scores[i].score = 0;
				app->games[k].top_scores[i].time = 0;
				app->games[k].top_scores[i].status = 0;
				strcpy(app->games[k].top_scores[i].player, "-");
			}
		}
		load_crt_tops(app);
		save_top_score(app);
	} else {
		fclose(f);
		f = fopen(SCORE_FILE, "r");
		for (k = 3; k < MAX_DIM; k++) {
			for (i = 0; i < SCORES; i++) {
				fscanf(f, "%d", &app->games[k].top_scores[i].score);
				fscanf(f, "%d", &app->games[k].top_scores[i].time);
				fscanf(f, "%d", &app->games[k].top_scores[i].status);
				fscanf(f, "%s", app->games[k].top_scores[i].player);
			}
		}
		load_crt_tops(app);
		fclose(f);
	}
}
