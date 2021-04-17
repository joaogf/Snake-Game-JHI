#ifndef GAME_SYSTEM_VARS_H
#define GAME_SYSTEM_VARS_H

#include "utils.h"
#include "player.h"
#include "jhi_sound.h"

void set_number_of_players(int num);

int get_number_of_players();

Player* get_player_by_index(int index);

void load_music_title_screen();

void play_title_music();

void free_title_music();




#endif
