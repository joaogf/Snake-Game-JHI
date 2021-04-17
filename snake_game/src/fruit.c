#include "fruit.h"


void init_fruit(Fruit *fr)
{

	int value;
	srand(time(0));
	value = rand() % num_fruits;
	jhi_load_image_with_transparent_color(&(fr->fruit_image), fruit_map[value].fruit_names, BLUE);
	fr->flag = fruit_map[value].flag;
	fr->points = fruit_map[value].points;
}

void change_fruit(Fruit *fr)
{
	int value = rand() % num_fruits;
	jhi_free_image(&(fr->fruit_image));
	jhi_load_image_with_transparent_color(&(fr->fruit_image), fruit_map[value].fruit_names, BLUE);
	fr->flag = fruit_map[value].flag;
	fr->points = fruit_map[value].points;
}

void set_fruit_position(Fruit *fr, JHI_Point2d pos, int w, int h, int block)
{
	fr->fruit_image.pos.x = RAND_INTERVAL(pos.x + block, w + pos.x - block - FRUIT_WIDTH);

	fr->fruit_image.pos.y = RAND_INTERVAL(pos.y + block, h + pos.y - block - FRUIT_HEIGHT);

}

void draw_fruit(Fruit *fr)
{
	jhi_draw_image(&(fr->fruit_image), fr->fruit_image.pos);
}

void free_fruit(Fruit *fr)
{
	jhi_free_image(&(fr->fruit_image));
}
