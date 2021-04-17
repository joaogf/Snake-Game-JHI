#ifndef UTILS_H
#define UTILS_H

#include "jhi_shapes.h"
#include "fruit.h"
#include "jhi_keyboard.h"
#include "jhi_joystick.h"
#include "snake.h"

#define WALL_FILE_PATH "../images/wall.png"
#define BACK_FILE_PATH "../images/environment_forest_evening.png"
#define	SNAKE_DEATH_FILE_PATH "../images/snake_death_img.png"
#define SNAKE_DEATH_FILE_PATH2 "../images/snake_death_img2.png"
#define SNAKE_DEATH_FILE_PATH3 "../images/snake_death_img3.png"
#define LOGO_LIB				"../images/Logotipo.png"
#define	SNAKE_FILE_PATH "../images/snake_img.png"
#define BUTTON_FONT "../font/Chewy.ttf"
#define BUTTON_SET_UP "../images/set_up.png"
#define BUTTON_SET_DOWN "../images/set_down.png"
#define BUTTON_SET_RED_UP "../images/set_up_red.png"
#define BUTTON_SET_RED_DOWN "../images/set_down_red.png"
#define REGULAR_FONT "../font/Chewy.ttf"
#define MUSIC_PATH "../sound/N.C.H-thoughts.mp3"
#define CLICK_BOTTON_SOUND "../sound/SFX_Powerup_05.wav"
#define MONSTER_FONT "../font/Chewy.ttf"
#define TITLE_MUSIC "../sound/N.C.H-cheerfulbgmStabbedbyMemories.mp3"
#define COLID_SOUND "../sound/qubodup-crash.ogg"
#define EAT_SOUND "../sound/SFX_Powerup_14.wav"
#define COLID_DEATH 1
#define COLID_FRUIT 2
#define NOT_COLID 0
#define SNAKE_HEAD_COLID 4
#define BLOCK_LENGTH 32
#define SCENARIO_WIDTH 640
#define SCENARIO_HEIGHT 400


JHI_Point2d get_point(int x, int y);

int num_of_digits(int num);

JHI_Keys convert_joy_to_key(JHI_JOY_DIR_ dir);

#endif
