#ifndef RANK_H
#define RANK_H

#define NUM_RANK 8
#define NAME_LENGHT 11

#include <stdio.h>
#include <string.h>
#include "jhi_font.h"
#include "jhi_text.h"
#include "utils.h"
#include "handle_menu.h"

typedef struct{
	char name[NAME_LENGHT];
	int points;
}RankSt;

static const char rank_levels_files[][30] = {
		"../input/rank_save_easy",
		"../input/rank_save_normal",
		"../input/rank_save_hard"
};

void add_rank(const char* name, int points, GameLevel level);

void draw_rank(JHI_Font *f, GameLevel level);


#endif
