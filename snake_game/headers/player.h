#ifndef PLAYER_H
#define PLAYER_H

#include <string.h>
#include <malloc.h>
#include "jhi_text.h"
typedef struct
{
	int points;
	int is_death;
	int cnt_victorys;
	JHI_Text txt;
	char name[12];
}Player;


void init_player(Player *p, JHI_Font *f, const char *name, int cnt_victorys);

void player_increment_points(Player *p,  JHI_Font *f, int point);


#endif
