#ifndef HANDLE_MENU_H
#define HANDLE_MENU_H

#include "button.h"
#include "jhi_keyboard.h"

#define DRAW_ARROWS draw_arrows(but_up_pointer, but_down_pointer, num_of_ops, \
				 init_arrow_pos, text_pointer, text_pointer_name)

#define HANDLE_ARROWS handle_arrows(mouse, but_up_pointer, but_down_pointer,\
					&but_up_red, &but_up, &but_down_red, &but_down,\
					num_of_ops, init_arrow_pos, text_pointer_name,\
					&f1, vet_arrow_type)

typedef enum
{
	EASY = 0,
	NORMAL,
	HARD
}GameLevel;

typedef enum
{
	KEYBOARD = 0,
	JOYSTICK
}ControlType;

typedef enum
{
	RANK_LEVEL = 0,
	LEVEL,
	CONTROL_TYPE
}ArrowOp;

typedef enum
{
	IS_TITLE = 0,
	IS_DEATH,
	IS_DEATH2,
	IS_DEATH3,
	IS_PLAY,
	IS_PLAY2,
	IS_PLAY3,
	IS_PLAY4,
	IS_INPUT,
	IS_CONF,
	IS_RANK,
	IS_CLOSE,
	IS_INTRO
}TypeScreen;


static const char levels[][10] = {
		"Facil",
		"Normal",
		"Dificil"
};

static const char control[][10] = {
		"Teclado",
		"Joystick"
};

static const int frames_value[] = {
		10,
		20,
		30
};


void draw_arrows(JHI_Image **but_up_pointer, JHI_Image **but_down_pointer, int num_of_ops,
		JHI_Point2d init_arrow_pos, JHI_Text **text_pointer, JHI_Text **text_pointer_name);

void handle_arrows(JHI_MouseSt mouse, JHI_Image **but_up_pointer, JHI_Image **but_down_pointer,
		JHI_Image *but_up_red, JHI_Image *but_up, JHI_Image *but_down_red, JHI_Image *but_down,
		int num_of_ops, JHI_Point2d init_arrow_pos, JHI_Text **text_pointer_name,
		JHI_Font *f1, ArrowOp *vet_arrow_type);

int check_buttons(JHI_MouseSt mouse, Button *vet, int vet_size);

char check_input(Button *but, JHI_MouseSt mouse, char *flag, int length);

int get_frames_per_seconds();

GameLevel get_game_level();

GameLevel get_rank_game_level();

ControlType get_control_game_type();

#endif
