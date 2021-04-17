#include "game_system_vars.h"

static Player Player_Var[3];
static int number_players = 3;
static JHI_Music Title_Screen_Music;

void set_number_of_players(int num)
{
	number_players = num;
}

int get_number_of_players()
{
	return number_players;
}

Player* get_player_by_index(int index)
{
	return &Player_Var[index];
}


void load_music_title_screen()
{
	jhi_load_music(&Title_Screen_Music, TITLE_MUSIC);
}

void play_title_music()
{
	jhi_play_music(&Title_Screen_Music, -1);
}

void free_title_music()
{
	jhi_free_music(&Title_Screen_Music);
}
