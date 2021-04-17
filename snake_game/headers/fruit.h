#ifndef FRUIT_H
#define FRUIT_H

#define FRUIT_HEIGHT 25
#define FRUIT_WIDTH 25

#include "jhi_image.h"
#include "jhi_window.h"
#include "jhi_rand.h"


typedef struct {
	char fruit_names[30];
	int points;
	int flag;

}FruitMap;

typedef struct
{
	int flag;
	int points;
	JHI_Image fruit_image;
}Fruit;

static const int num_fruits = 8;

static FruitMap fruit_map[] = {
		{.fruit_names = "../images/sprite_fruit.png", 10, 1},
		{.fruit_names = "../images/sprite_fruit2.png",0, 0},
		{.fruit_names = "../images/sprite_fruit3.png",8, 1},
		{.fruit_names = "../images/sprite_fruit4.png",0, 0},
		{.fruit_names = "../images/sprite_fruit5.png", 5, 1},
		{.fruit_names = "../images/sprite_fruit6.png",6, 1},
		{.fruit_names = "../images/sprite_fruit7.png",20, 1},
		{.fruit_names = "../images/sprite_fruit8.png",25, 1}
};

void init_fruit(Fruit *fr);

void change_fruit(Fruit *fr);

void set_fruit_position(Fruit *fr, JHI_Point2d pos, int w, int h, int block);

void draw_fruit(Fruit *fr);

void free_fruit(Fruit *fr);


#endif
