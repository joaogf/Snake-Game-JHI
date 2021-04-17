#ifndef GAME_H
#define GAME_H

#include "snake.h"
#include "jhi_window.h"
#include "fruit.h"
#include "jhi_sound.h"
#include "jhi_mouse.h"
#include "jhi_keyboard.h"
#include "jhi_shapes.h"
#include "button.h"
#include "jhi_joystick.h"
#include "handle_menu.h"
#include "player.h"
#include "game_system_vars.h"

#define GAME_EXEC(t)\
	while(1)\
	{\
		t = game_funcs[t].f();\
		if (t == IS_CLOSE)\
		{\
			break;\
		}\
	}\

#define EXEC_WITH_DEATH_CHECK(player,cmd)\
	if (player->is_death == 0)\
		cmd



typedef struct{
	TypeScreen (*f)();
}GameFunctionsMap;


TypeScreen game_play_scenario();

TypeScreen game_play_scenario_with_two_players();

TypeScreen game_play_scenario_with_three_players();

TypeScreen game_play_scenario_with_cpu();

TypeScreen game_conf_screen();

TypeScreen game_death_screen();

TypeScreen game_death_screen2();

TypeScreen game_death_screen3();

TypeScreen game_title_screen();

TypeScreen game_rank_screen();

TypeScreen game_input_name_player_screen();

TypeScreen game_get_close();

TypeScreen game_intro_screen();

static const GameFunctionsMap game_funcs[] = {
		{.f = game_title_screen},
		{.f = game_death_screen},
		{.f = game_death_screen2},
		{.f = game_death_screen3},
		{.f = game_play_scenario},
		{.f = game_play_scenario_with_two_players},
		{.f = game_play_scenario_with_cpu},
		{.f = game_play_scenario_with_three_players},
		{.f = game_input_name_player_screen},
		{.f = game_conf_screen},
		{.f = game_rank_screen},
		{.f = game_get_close},
		{.f = game_intro_screen}
};

#endif
