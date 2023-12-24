#include "theme.h"

int read_themes(theme themes[])
{
	FILE *f;
	int i;
	int color_id_start = 31;
	int count = 0;
	f = fopen("colors.txt", "r");
	if (f == NULL)
		printf("Color file not found!");
	else {
		while(!feof(f)) {
			fscanf(f, "%s", themes[count].name);
			for (i = 0; i < 11; i++) {
				themes[count].color[i].id = color_id_start + count * 11 + i;
				fscanf(f, "%d", &themes[count].color[i].r);
				fscanf(f, "%d", &themes[count].color[i].g);
				fscanf(f, "%d", &themes[count].color[i].b);
				themes[count].color[i].r = (int)(themes[count].color[i].r / 256.0 * 1000);
				themes[count].color[i].g = (int)(themes[count].color[i].g / 256.0 * 1000);
				themes[count].color[i].b = (int)(themes[count].color[i].b / 256.0 * 1000);
			}
			count++;
		}
		fclose(f);
	}
	for (i = 0; i < count; i++)
		initialize_colors(themes[i]);
	return count;
}

void initialize_colors(theme theme)
{
	int i;
	// create colors
	for (i = 0; i < 11; i++) {
		init_color(theme.color[i].id, theme.color[i].r, theme.color[i].g, theme.color[i].b);
		init_pair(theme.color[i].id, COLOR_BLUE, theme.color[i].id);
	}
	init_color(12, 700, 700, 850);   // light blue // not selected
	init_color(11, 520, 600, 960);   // dark blue // selected
	init_pair(11, 11, 11); // selected
	init_pair(12, 12, 12); // not selected
}

void set_theme(theme theme)
{
	int i;
	for (i = 0; i < 11; i++) {
		init_color(20 + i, theme.color[i].r, theme.color[i].g, theme.color[i].b);
		init_pair(20 + i, COLOR_BLUE, theme.color[i].id);
	}
}

