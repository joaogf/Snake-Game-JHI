#include "rank.h"


static void change_rank_pos(RankSt *r1, RankSt *r2)
{
	int aux;
	char name_aux[NAME_LENGHT];

	memset(name_aux, 0, sizeof(name_aux));

	aux = r1->points;
	r1->points = r2->points;
	r2->points = aux;

	strncpy(name_aux, r1->name, sizeof(name_aux));
	strncpy(r1->name, r2->name, sizeof(r1->name));
	strncpy(r2->name, name_aux, sizeof(r2->name));
}

static void sort_rank(RankSt *vet, int size){
	int i,j;
	int max;

	for(i = 0; i < size-1; i++){
		max = i;
		for(j = i+1; j < size; j++){
			if (vet[j].points > vet[max].points) max=j;
		}
		if (i != max){
			change_rank_pos(&vet[i], &vet[max]);
		}

	}


}


void add_rank(const char* name, int points, GameLevel level)
{
	int i;
	RankSt r_vet[NUM_RANK + 1];
	FILE *fp = fopen(rank_levels_files[level], "r+");

	for (i = 0; i < NUM_RANK; i++)
	{
		memset(r_vet[i].name, 0, sizeof(r_vet[i].name));
		fscanf(fp, "%s %d", r_vet[i].name, &r_vet[i].points);
	}



	memset(r_vet[NUM_RANK].name, 0, sizeof(r_vet[NUM_RANK].name));
	strncpy(r_vet[NUM_RANK].name, name, sizeof(r_vet[NUM_RANK].name));
	r_vet[NUM_RANK].points = points;

	sort_rank(r_vet, NUM_RANK + 1);

	rewind(fp);

	for (i = 0; i < NUM_RANK; i++)
	{
		fprintf(fp, "%s %d\n", r_vet[i].name, r_vet[i].points);
	}

	fclose(fp);
}


static void set_w_h_text(FILE *fp, JHI_Font *f, int *w, int *h)
{
	JHI_Text line;
	int i;
	char line_str[NAME_LENGHT + 15];
	RankSt rank;
	jhi_init_text(&line);
	*w = 0;
	*h = 0;

	for (i = 0; i < NUM_RANK; i++)
	{
		memset(rank.name, 0, sizeof(rank.name));
		memset(line_str, 0, sizeof(line_str));
		fscanf(fp, "%s %d", rank.name, &rank.points);
		snprintf(line_str, sizeof(line_str), "%d - %s: %d", i + 2, rank.name, rank.points);
		jhi_set_text(f, &line, WHITE, line_str);

		if (jhi_get_text_width(&line) > *w)
		{
			*w = jhi_get_text_width(&line);
		}

		if (jhi_get_text_height(&line) > *h)
		{
			*h = jhi_get_text_height(&line);
		}
	}

	*w += 20;

	rewind(fp);
}

void draw_rank(JHI_Font *f, GameLevel level)
{
	JHI_Text line;
	int i,w,h;
	char line_str[NAME_LENGHT + 15];
	RankSt rank;
	jhi_init_text(&line);

	FILE *fp = fopen(rank_levels_files[level], "r");


	set_w_h_text(fp, f, &w, &h);


	memset(rank.name, 0, sizeof(rank.name));
	memset(line_str, 0, sizeof(line_str));
	fscanf(fp, "%s %d", rank.name, &rank.points);
	snprintf(line_str, sizeof(line_str), "1 - %s: %d", rank.name, rank.points);
	jhi_set_text(f, &line, WHITE, line_str);

	line.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), jhi_get_text_width(&line), 100).x;
	line.pos.y = 100;

	jhi_draw_fill_rect(line.pos, h, w, BLACK);
	jhi_draw_rect(line.pos, h, w, RED);
	jhi_draw_text(&line, line.pos);


	for (i = 0; i < NUM_RANK - 1; i++)
	{
		memset(rank.name, 0, sizeof(rank.name));
		memset(line_str, 0, sizeof(line_str));
		fscanf(fp, "%s %d", rank.name, &rank.points);
		snprintf(line_str, sizeof(line_str), "%d - %s: %d", i + 2, rank.name, rank.points);
		jhi_set_text(f, &line, WHITE, line_str);
		line.pos.y += jhi_get_text_height(&line);
		jhi_draw_fill_rect(line.pos, h, w, BLACK);
		jhi_draw_rect(line.pos, h, w, RED);
		jhi_draw_text(&line, line.pos);

	}

	jhi_free_text(&line);
	fclose(fp);

}
