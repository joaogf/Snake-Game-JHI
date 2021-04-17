#include "player.h"


void init_player(Player *p, JHI_Font *f, const char *name, int cnt_victorys)
{
	char to_txt[20];
	p->points = 0;
	memset(p->name, 0, sizeof(p->name));
	strncpy(p->name, name, sizeof(p->name));
	memset(to_txt, 0, sizeof(to_txt));
	strncpy(to_txt, p->name, sizeof(to_txt));
	strncat(to_txt, "  0", sizeof(char)*3);
	jhi_init_text(&(p->txt));
	jhi_set_text(f, &(p->txt), GREEN, to_txt);
	p->is_death = 0;
	p->cnt_victorys = cnt_victorys;
}


void player_increment_points(Player *p,  JHI_Font *f, int point)
{
	int old_points = p->points;
	int length;
	p->points += point;

	char *to_txt;
	length = sizeof(char) * (num_of_digits(p->points) + 1 + strlen(p->name));
	to_txt = (char *)malloc(length);
	memset(to_txt, 0, length);
	strncpy(to_txt, p->name, length);
	sprintf(to_txt + strlen(p->name), " %d", p->points);
	jhi_set_text(f, &(p->txt), GREEN, to_txt);

	free(to_txt);
}

void free_player(Player *p)
{
	jhi_free_text(&(p->txt));
}

void draw_player(Player *p, int x, int y)
{
	p->txt.pos.x = x;
	p->txt.pos.y = y;


	jhi_draw_rect(p->txt.pos, jhi_get_text_height(&(p->txt)), jhi_get_text_width(&(p->txt)), WHITE);
	jhi_draw_text(&(p->txt), p->txt.pos);
}
