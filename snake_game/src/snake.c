#include "snake.h"


static SnakeFlagDir snake_flag_map[] = {
		{KEY_UP, 1}, {KEY_DOWN, 1},
		{KEY_RIGHT, 1}, {KEY_LEFT, 1}
};

static JHI_Keys get_next_random_cpu_dir()
{
	JHI_Keys dir;
	int pos;
	int i;
	dir = RAND_INTERVAL(KEY_UP, KEY_LEFT);

	pos = dir - KEY_UP;

	for (i = 0; i < 5; i++)
	{

		if (snake_flag_map[pos].flags)
		{
			snake_flag_map[pos].flags = 0;
			return dir;
		}

		pos++;
		pos %= 5;
		dir = pos + KEY_UP;
	}

	return NO_KEY;
}

static void init_snake_flags(JHI_Keys cur_dir)
{
	JHI_Keys opposite_key = jhi_get_opposite_key_arrow(cur_dir);
	int i;

	for (i = 0; i < 4; i++)
	{
		snake_flag_map[i].flags = 1;
	}

	snake_flag_map[opposite_key - KEY_UP].flags = 0;
}

static void move_piece(JHI_Point2d *pos, JHI_Point2d vel, JHI_Keys dir)
{
	switch(dir)
	{
		case KEY_DOWN:
			pos->y += vel.y;
			break;
		case KEY_UP:
			pos->y -= vel.y;
			break;
		case KEY_RIGHT:
			pos->x += vel.x;
			break;
		case KEY_LEFT:
			pos->x -= vel.x;
			break;
	}
}

static void move_tail(Snake *sn)
{
	int i;

	if(sn->tail_lenght == 0)
	{
		return;
	}


	move_piece(&(sn->tail_pos[0].pos), sn->vel, sn->tail_pos[0].dir);

	for(i = sn->tail_lenght - 1;  i > 0; i--)
	{
		move_piece(&(sn->tail_pos[i].pos), sn->vel, sn->tail_pos[i].dir);

		sn->tail_pos[i].dir = sn->tail_pos[i-1].dir;

	}

	sn->tail_pos[0].dir = sn->dir;

}

static void change_dir_snake(Snake *sn, JHI_Keys dir)
{
	switch(sn->dir)
	{
		case KEY_DOWN:
			if (dir == KEY_RIGHT || dir == KEY_LEFT) sn->dir = dir;
			break;
		case KEY_UP:
			if (dir == KEY_RIGHT || dir == KEY_LEFT) sn->dir = dir;
			break;
		case KEY_RIGHT:
			if (dir == KEY_UP || dir == KEY_DOWN) sn->dir = dir;
			break;
		case KEY_LEFT:
			if (dir == KEY_UP || dir == KEY_DOWN) sn->dir = dir;
			break;
	}
}

void init_snake(Snake *sn, int lenght, JHI_Point2d pos, JHI_Point2d vel, JHI_Color snake_color)
{
	int i;

	sn->tail_lenght = lenght;
	sn->head_pos = pos;
	sn->vel = vel;
	sn->snake_color = snake_color;
	sn->dir = KEY_RIGHT;

	for (i = 0; i < lenght; i++)
	{
		sn->tail_pos[i].pos.x = pos.x - (RECT_SNAKE_LENGHT *(i+1));
		sn->tail_pos[i].pos.y = pos.y;
		sn->tail_pos[i].dir = KEY_RIGHT;
	}

	sn->cpu_cnt = 0;
	sn->eat_now = lenght;
	sn->eat_control = jhi_get_fps_timer();

	sn->ani_index = 0;
	sn->ani_control = 0;
	jhi_load_image_with_transparent_color(&sn->ani_death, "../images/efect_chock.png", BACK_CHOCK);
}


void move_snake(Snake *sn, JHI_Keys dir)
{

	change_dir_snake(sn, dir);

	move_piece(&(sn->head_pos), sn->vel, sn->dir);

	move_tail(sn);

}

int snake_check_all_colision(Snake *sn, JHI_Image *wall, Fruit *fr)
{
	if (is_colid_with_tail(sn))
	{
		return COLID_DEATH;
	}

	if (snake_check_colosion_wall(sn, wall->pos, jhi_get_image_width(wall),
			jhi_get_image_height(wall), BLOCK_LENGTH))
	{
		return COLID_DEATH;
	}

	if (jhi_is_colid(sn->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT,
			fr->fruit_image.pos, FRUIT_HEIGHT, FRUIT_WIDTH))
	{
		return COLID_FRUIT;
	}

	return NOT_COLID;
}

void snake_eat(Snake *sn)
{
	sn->tail_lenght++;

	if(sn->tail_lenght == 1)
	{
		switch(sn->dir)
		{
			case KEY_DOWN:
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->head_pos.y - RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->head_pos.x;
				break;
			case KEY_UP:
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->head_pos.y + RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->head_pos.x;
				break;
			case KEY_RIGHT:
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->head_pos.x - RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->head_pos.y;
				break;
			case KEY_LEFT:
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->head_pos.x + RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->head_pos.y;
				break;
		}

		sn->tail_pos[sn->tail_lenght - 1].dir = sn->dir;
	}
	else
	{
		switch(sn->tail_pos[sn->tail_lenght - 2].dir)
		{
			case KEY_DOWN:
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->tail_pos[sn->tail_lenght - 2].pos.y - RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->tail_pos[sn->tail_lenght - 2].pos.x;
				break;
			case KEY_UP:
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->tail_pos[sn->tail_lenght - 2].pos.y + RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->tail_pos[sn->tail_lenght - 2].pos.x;
				break;
			case KEY_RIGHT:
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->tail_pos[sn->tail_lenght - 2].pos.x - RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->tail_pos[sn->tail_lenght - 2].pos.y;
				break;
			case KEY_LEFT:
				sn->tail_pos[sn->tail_lenght - 1].pos.x = sn->tail_pos[sn->tail_lenght - 2].pos.x + RECT_SNAKE_LENGHT;
				sn->tail_pos[sn->tail_lenght - 1].pos.y = sn->tail_pos[sn->tail_lenght - 2].pos.y;
				break;
		}

		sn->tail_pos[sn->tail_lenght - 1].dir = sn->tail_pos[sn->tail_lenght - 2].dir;
	}

	sn->eat_now = 0;
	sn->eat_control = 0;
}

int is_colid_with_tail(Snake *sn)
{

	int i;

	for(i = 0; i < sn->tail_lenght; i++)
	{
		if (jhi_is_colid(sn->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT,
				sn->tail_pos[i].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT))
		{
			return 1;
		}
	}

	return 0;
}

int is_colid_with_another_snake(Snake *sn1, Snake *sn2)
{

	int i;

	if (jhi_is_colid(sn1->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT,
				sn2->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT))
	{
		return SNAKE_HEAD_COLID;
	}

	for(i = 0; i < sn2->tail_lenght; i++)
	{
		if (jhi_is_colid(sn1->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT,
				sn2->tail_pos[i].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT))
		{
			return 1;
		}
	}

	return 0;
}

int snake_check_colosion_wall(Snake *sn, JHI_Point2d pos, int w, int h, int block)
{
	if ( !(pos.y + block <= sn->head_pos.y && sn->head_pos.y + RECT_SNAKE_LENGHT <= pos.y + h - block))
	{
		return 1;
	}


	if ( !(pos.x + block <= sn->head_pos.x && sn->head_pos.x + RECT_SNAKE_LENGHT <= pos.x + w - block))
	{
		return 1;
	}

	return 0;
}

int draw_snake_anim(Snake *sn)
{

	if (sn->ani_index == 3) { return 1;}

	JHI_Point2d clip;

	int width = jhi_get_image_width(&sn->ani_death) / SNAKE_NUM_ANIM_CLIP;
	int height = jhi_get_image_height(&sn->ani_death);

	clip.y = 0;
	clip.x = width * sn->ani_index;

	jhi_draw_image_with_clip(&sn->ani_death, sn->head_pos, clip,
			width, height);

	if (sn->ani_control == 10) {
		sn->ani_index++;
		sn->ani_control = 0;
	}
	else {
		sn->ani_control++;
	}

	if (sn->ani_index == 3) { return 1;}

	return 0;
}

void draw_snake(Snake *sn)
{
	int i;

	//Draw Head
	jhi_draw_fill_rect(sn->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, sn->snake_color);
	jhi_draw_rect(sn->head_pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, YELLOW);


	//Draw Tail after piece one
	for (i = 0; i < sn->tail_lenght; i++)
	{
		jhi_draw_fill_rect(sn->tail_pos[i].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, sn->snake_color);
		jhi_draw_rect(sn->tail_pos[i].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, BLUE);
	}

	if (sn->eat_now < sn->tail_lenght)
	{
		jhi_draw_fill_rect(sn->tail_pos[sn->eat_now].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, SNAKE_YELLOW);
		jhi_draw_rect(sn->tail_pos[sn->eat_now].pos, RECT_SNAKE_LENGHT, RECT_SNAKE_LENGHT, BLUE);
		if(sn->eat_control > jhi_get_fps_timer())
		{
			sn->eat_now++;
			sn->eat_control = 0;
		}
		sn->eat_control+=15;
	}

}

JHI_Keys snake_random_cpu(Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall)
{
	JHI_Keys dir;
	JHI_Point2d backup_pos;
	int colid;
	int flag_exit = 1;
	backup_pos = sn2->head_pos;
	init_snake_flags(sn2->dir);

	move_piece(&sn2->head_pos, sn2->vel, sn2->dir);
	CHECK_CPU_COLID_SNAKE(colid, flag_exit);
	if (flag_exit && RAND_01 <= CPU_SNAKE_NOT_CHANGE_DIR) {sn2->head_pos = backup_pos; return sn2->dir;}
	snake_flag_map[sn2->dir - KEY_UP].flags = 0;
	sn2->head_pos = backup_pos;

	do
	{

		dir = get_next_random_cpu_dir();
		if (RAND_01 <= CPU_SNAKE_ERROR || dir == NO_KEY) break;
		move_piece(&sn2->head_pos, sn2->vel, dir);
		CHECK_CPU_COLID_SNAKE(colid, flag_exit);
		sn2->head_pos = backup_pos;
	}while(!flag_exit);

	sn2->head_pos = backup_pos;
	return dir;
}

JHI_Keys snake_random_cpu_two_snakes(Snake *sn1, Snake *sn2, Snake *sn3, Fruit *fr, JHI_Image *wall)
{
	JHI_Keys dir;
	JHI_Point2d backup_pos;
	int colid;
	int flag_exit = 1;
	backup_pos = sn3->head_pos;
	init_snake_flags(sn3->dir);

	move_piece(&sn3->head_pos, sn3->vel, sn3->dir);
	CHECK_CPU_COLID_TWO_SNAKE(colid, flag_exit);
	if (flag_exit && RAND_01 <= CPU_SNAKE_NOT_CHANGE_DIR) {sn3->head_pos = backup_pos; return sn3->dir;}
	snake_flag_map[sn3->dir - KEY_UP].flags = 0;
	sn3->head_pos = backup_pos;

	do
	{

		dir = get_next_random_cpu_dir();
		if (RAND_01 <= CPU_SNAKE_ERROR || dir == NO_KEY) break;
		move_piece(&sn3->head_pos, sn3->vel, dir);
		CHECK_CPU_COLID_TWO_SNAKE(colid, flag_exit);
		sn3->head_pos = backup_pos;
	}while(!flag_exit);

	sn3->head_pos = backup_pos;
	return dir;
}

JHI_Keys snake_greedy_cpu(Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall)
{
	JHI_Keys dir_fruit_y = NO_KEY;
	JHI_Keys dir_fruit_x = NO_KEY;
	JHI_Point2d backup_pos = sn2->head_pos;
	int colid, flag_exit;

	//not in the same x
	if (sn2->head_pos.x + RECT_SNAKE_LENGHT <= fr->fruit_image.pos.x
			|| fr->fruit_image.pos.x + jhi_get_image_width(&fr->fruit_image) <= sn2->head_pos.x)
	{
		if (sn2->head_pos.x < fr->fruit_image.pos.x){dir_fruit_x = KEY_RIGHT;}
		else {dir_fruit_x = KEY_LEFT;}
	}

	//not in the same y
	if (sn2->head_pos.y + RECT_SNAKE_LENGHT <= fr->fruit_image.pos.y
			|| fr->fruit_image.pos.y + jhi_get_image_height(&fr->fruit_image) <= sn2->head_pos.y)
	{
		if (sn2->head_pos.y < fr->fruit_image.pos.y){dir_fruit_y = KEY_DOWN;}
		else {dir_fruit_y = KEY_UP;}
	}

	if (dir_fruit_x != NO_KEY && dir_fruit_x != jhi_get_opposite_key_arrow(sn2->dir))
	{
		move_piece(&sn2->head_pos, sn2->vel, dir_fruit_x);
		CHECK_CPU_COLID_SNAKE(colid, flag_exit)
		if(flag_exit || RAND_01 <= CPU_SNAKE_ERROR) {sn2->head_pos = backup_pos; return dir_fruit_x;}
	}

	sn2->head_pos = backup_pos;

	if (dir_fruit_y != NO_KEY && dir_fruit_y != jhi_get_opposite_key_arrow(sn2->dir))
	{
		move_piece(&sn2->head_pos, sn2->vel, dir_fruit_y);
		CHECK_CPU_COLID_SNAKE(colid, flag_exit)
		if(flag_exit || RAND_01 <= CPU_SNAKE_ERROR) {sn2->head_pos = backup_pos; return dir_fruit_y;}
	}

	sn2->head_pos = backup_pos;

	return snake_random_cpu(sn1, sn2, fr, wall);

}

JHI_Keys snake_greedy_cpu_two_snakes(Snake *sn1, Snake *sn2, Snake *sn3, Fruit *fr, JHI_Image *wall)
{
	JHI_Keys dir_fruit_y = NO_KEY;
	JHI_Keys dir_fruit_x = NO_KEY;
	JHI_Point2d backup_pos = sn3->head_pos;
	int colid, flag_exit;

	//not in the same x
	if (sn3->head_pos.x + RECT_SNAKE_LENGHT <= fr->fruit_image.pos.x
			|| fr->fruit_image.pos.x + jhi_get_image_width(&fr->fruit_image) <= sn3->head_pos.x)
	{
		if (sn3->head_pos.x < fr->fruit_image.pos.x){dir_fruit_x = KEY_RIGHT;}
		else {dir_fruit_x = KEY_LEFT;}
	}

	//not in the same y
	if (sn3->head_pos.y + RECT_SNAKE_LENGHT <= fr->fruit_image.pos.y
			|| fr->fruit_image.pos.y + jhi_get_image_height(&fr->fruit_image) <= sn3->head_pos.y)
	{
		if (sn3->head_pos.y < fr->fruit_image.pos.y){dir_fruit_y = KEY_DOWN;}
		else {dir_fruit_y = KEY_UP;}
	}

	if (dir_fruit_x != NO_KEY && dir_fruit_x != jhi_get_opposite_key_arrow(sn3->dir))
	{
		move_piece(&sn3->head_pos, sn3->vel, dir_fruit_x);
		CHECK_CPU_COLID_TWO_SNAKE(colid, flag_exit)
		if(flag_exit || RAND_01 <= CPU_SNAKE_ERROR) {sn3->head_pos = backup_pos; return dir_fruit_x;}
	}

	sn3->head_pos = backup_pos;

	if (dir_fruit_y != NO_KEY && dir_fruit_y != jhi_get_opposite_key_arrow(sn3->dir))
	{
		move_piece(&sn3->head_pos, sn3->vel, dir_fruit_y);
		CHECK_CPU_COLID_TWO_SNAKE(colid, flag_exit)
		if(flag_exit || RAND_01 <= CPU_SNAKE_ERROR) {sn3->head_pos = backup_pos; return dir_fruit_y;}
	}

	sn3->head_pos = backup_pos;

	return snake_random_cpu_two_snakes(sn1, sn2, sn3, fr, wall);

}

JHI_Keys get_snake_cpu_dir(SnakeCpuType s_type, Snake *sn1, Snake *sn2, Fruit *fr, JHI_Image *wall)
{
	return snake_cpu_funcs[s_type].f(sn1, sn2, fr, wall);
}

JHI_Keys get_snake_cpu_dir_two_snakes(SnakeCpuType s_type, Snake *sn1, Snake *sn2, Snake *sn3, Fruit *fr, JHI_Image *wall)
{
	return snake_greedy_cpu_two_snakes(sn1, sn2, sn3, fr, wall);
}

void free_snake(Snake *sn)
{
	jhi_free_image(&sn->ani_death);
}
