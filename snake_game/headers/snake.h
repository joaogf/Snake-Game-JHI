#ifndef SNAKE_H
#define SNAKE_H

#define MAX_TAIL_LENGHT 1000
#define RECT_SNAKE_LENGHT 15
#define SNAKE_NUM_ANIM_CLIP 3
#define CPU_SNAKE_ERROR 0.001
#define CPU_SNAKE_NOT_CHANGE_DIR 0.9

#include "jhi_keyboard.h"
#include "jhi_colorsSt.h"
#include "jhi_window.h"
#include "utils.h"


typedef enum
{
	SNAKETYPE1 = 0,
	SNAKETYPE2
}SnakeCpuType;


typedef struct {
	JHI_Point2d pos;
	JHI_Keys dir;
}TailSt;

typedef struct {
	JHI_Point2d head_pos;
	JHI_Point2d vel;
	int tail_lenght;
	JHI_Keys dir;
	TailSt tail_pos[MAX_TAIL_LENGHT];
	JHI_Color snake_color;
	int eat_now;
	int eat_control;
	int cpu_cnt;
	JHI_Image ani_death;
	int ani_index;
	int ani_control;
}Snake;

typedef struct {
	JHI_Keys dirs;
	int flags;
}SnakeFlagDir;

typedef struct{
	JHI_Keys (*f)(Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall);
}SnakeCpuFunctionsMap;


#define CHECK_CPU_COLID_SNAKE(colid, flag_exit) \
		colid = snake_check_all_colision(sn2, wall, fr);\
		flag_exit = ! ((colid == COLID_DEATH) || (colid == COLID_FRUIT && !fr->flag) ||\
				(is_colid_with_another_snake(sn1, sn2)));\

#define CHECK_CPU_COLID_TWO_SNAKE(colid, flag_exit) \
		colid = snake_check_all_colision(sn3, wall, fr);\
		flag_exit = ! ((colid == COLID_DEATH) || (colid == COLID_FRUIT && !fr->flag) ||\
				(is_colid_with_another_snake(sn1, sn3)) || (is_colid_with_another_snake(sn2, sn3)));\


void init_snake(Snake *sn, int lenght, JHI_Point2d pos, JHI_Point2d vel, JHI_Color snake_color);

void move_snake(Snake *sn, JHI_Keys dir);

int is_colid_with_tail(Snake *sn);

int is_colid_with_another_snake(Snake *sn1, Snake *sn2);

int snake_check_colosion_wall(Snake *sn, JHI_Point2d pos, int w, int h, int block);

int snake_check_all_colision(Snake *sn, JHI_Image *wall, Fruit *fr);

void snake_eat(Snake *sn);

int draw_snake_anim(Snake *sn);

void draw_snake(Snake *sn);

JHI_Keys get_snake_cpu_dir(SnakeCpuType s_type, Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall);

JHI_Keys snake_greedy_cpu(Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall);

JHI_Keys snake_random_cpu(Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall);

static const SnakeCpuFunctionsMap snake_cpu_funcs[] = {
		{.f = snake_random_cpu},
		{.f = snake_greedy_cpu}
};


#endif
