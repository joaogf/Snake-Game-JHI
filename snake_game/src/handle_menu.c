#include "handle_menu.h"


static int Frames_Per_Second = 20;
static GameLevel GL = NORMAL;
static GameLevel Rank_GL = NORMAL;
static ControlType CType = KEYBOARD;


static void set_next_op(JHI_Keys dir, unsigned int *op_type, int first_op, int last_op)
{
	if (dir == KEY_DOWN)
	{
		if (*op_type != first_op){ (*op_type)--;}
	}
	else
	{
		if (*op_type != last_op){(*op_type)++;}
	}
}

static void set_conf_by_arrow_pos(ArrowOp a_type, JHI_Text *text, JHI_Font *f, JHI_Keys dir)
{
	switch(a_type)
	{
		case RANK_LEVEL:
			set_next_op(dir, &Rank_GL, EASY, HARD);
			jhi_set_text(f, text, RED, levels[Rank_GL]);
			break;
		case LEVEL:
			set_next_op(dir, &GL, EASY, HARD);
			jhi_set_text(f, text, RED, levels[GL]);
			Frames_Per_Second = frames_value[GL];
			break;
		case CONTROL_TYPE:
			set_next_op(dir, &CType, KEYBOARD, JOYSTICK);
			jhi_set_text(f, text, RED, control[CType]);
			break;

	}
}


void draw_arrows(JHI_Image **but_up_pointer, JHI_Image **but_down_pointer, int num_of_ops,
		JHI_Point2d init_arrow_pos, JHI_Text **text_pointer, JHI_Text **text_pointer_name )
{
	int i;
	for (i = 0; i < num_of_ops; i++)
	{
		jhi_draw_image(but_up_pointer[i], jhi_get_point(init_arrow_pos.x,
				init_arrow_pos.y));

		init_arrow_pos.y += jhi_get_image_height(but_up_pointer[i]);

		jhi_draw_image(but_down_pointer[i], jhi_get_point(init_arrow_pos.x, init_arrow_pos.y));

		init_arrow_pos.y += jhi_get_image_height(but_down_pointer[i]);

		jhi_draw_text(text_pointer[i], jhi_get_point(init_arrow_pos.x + jhi_get_image_height(but_down_pointer[i]) + 15,
				init_arrow_pos.y - jhi_get_image_height(but_up_pointer[i]) - 15));
		jhi_draw_text(text_pointer_name[i], jhi_get_point(text_pointer[0]->pos.x +
				jhi_get_text_width(text_pointer[0]) + 15,
				 init_arrow_pos.y - jhi_get_image_height(but_down_pointer[i])- 15));
	}
}

void handle_arrows(JHI_MouseSt mouse, JHI_Image **but_up_pointer, JHI_Image **but_down_pointer,
		JHI_Image *but_up_red, JHI_Image *but_up, JHI_Image *but_down_red, JHI_Image *but_down,
		int num_of_ops, JHI_Point2d init_arrow_pos, JHI_Text **text_pointer_name,
		JHI_Font *f1, ArrowOp *vet_arrow_type)
{

	int i;

	for (i = 0; i < num_of_ops; i++)
	{

		if (jhi_check_mouse_is_in(mouse, init_arrow_pos.x, init_arrow_pos.y,
				jhi_get_image_width(but_up), jhi_get_image_height(but_up)))
		{
			but_up_pointer[i] = but_up_red;

			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				set_conf_by_arrow_pos(vet_arrow_type[i], text_pointer_name[i], f1, KEY_DOWN);
			}
		}
		else
		{
			but_up_pointer[i] = but_up;
		}

		init_arrow_pos.y += jhi_get_image_height(but_up_pointer[i]);

		if (jhi_check_mouse_is_in(mouse, init_arrow_pos.x, init_arrow_pos.y,
				jhi_get_image_width(but_down), jhi_get_image_height(but_down)))
		{
			but_down_pointer[i] = but_down_red;
			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				set_conf_by_arrow_pos(vet_arrow_type[i], text_pointer_name[i], f1, KEY_UP);
			}
		}
		else
		{
			but_down_pointer[i] = but_down;
		}

		init_arrow_pos.y += jhi_get_image_height(but_down_pointer[i]);
	}
}

int check_buttons(JHI_MouseSt mouse, Button *vet, int vet_size)
{
	int i, w,h;

	for (i = 0; i < vet_size; i++)
	{

		if (jhi_check_mouse_is_in(mouse, vet[i].pos.x, vet[i].pos.y,
				jhi_get_image_width(&(vet[i].img)), jhi_get_image_height(&(vet[i].img))))
		{

			w = jhi_get_image_width(&(vet[i].img));
			h = jhi_get_image_height(&(vet[i].img));

			if(change_button_image(&vet[i], BUTTON_TYPE_2))
			{
				set_button_size(&vet[i], w, h);
			}

			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				return i;
			}


		}

		else
		{
			w = jhi_get_image_width(&(vet[i].img));
			h = jhi_get_image_height(&(vet[i].img));

			if(change_button_image(&vet[i], BUTTON_TYPE_1))
			{
				set_button_size(&vet[i], w, h);
			}


		}
	}

	return -1;
}

char check_input(Button *but, JHI_MouseSt mouse, char *flag, int length)
{
	int i, w,h;

	char c = 'a';

	JHI_Point2d pos;

	pos = but->pos;

	while (c != 'z' + 1)
	{

		if (jhi_check_mouse_is_in(mouse, pos.x, pos.y,
				jhi_get_image_width(&(but->img)), jhi_get_image_height(&(but->img))))
		{

			*flag = c;

			if (mouse.mouse_event == ON_CLICK_LEFT)
			{
				return c;
			}


		}

		else if (*flag == c)
		{
			*flag = 'z' + 2;
		}

		pos.x += 3 * length;

		if (c == 'h' || c == 'p' || c =='x')
		{
			pos.x = 100;
			pos.y += 3 * length;
		}

		c++;

	}

	return 'z' + 2;
}

int get_frames_per_seconds()
{
	return Frames_Per_Second;
}

GameLevel get_game_level()
{
	return GL;
}
GameLevel get_rank_game_level()
{
	return Rank_GL;
}
ControlType get_control_game_type()
{
	return CType;
}
