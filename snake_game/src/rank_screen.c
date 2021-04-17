#include "game.h"


TypeScreen game_rank_screen()
{
	const int num_of_buttons = 1;
	int i, pos;
	const int num_of_ops = 1;
	ArrowOp vet_arrow_type[num_of_ops];
	JHI_Image but_up;
	JHI_Image but_down;
	JHI_Image but_up_red;
	JHI_Image but_down_red;
	JHI_Image *but_up_pointer[num_of_ops];
	JHI_Image *but_down_pointer[num_of_ops];
	JHI_Text * text_pointer[num_of_ops];
	JHI_Text * text_pointer_name[num_of_ops];
	JHI_Text level;
	JHI_Text level_name;
	JHI_Font f1,f2;
	TypeScreen te = IS_CLOSE;
	JHI_Image back;
	Button back_title[num_of_buttons];
	JHI_MouseSt mouse;
	JHI_Effect ef1;
	JHI_Point2d init_arrow_pos;

	jhi_load_image_with_transparent_color(&but_up, BUTTON_SET_UP, BLUE);
	jhi_load_image_with_transparent_color(&but_down, BUTTON_SET_DOWN, BLUE);
	jhi_load_image_with_transparent_color(&but_up_red, BUTTON_SET_RED_UP, BLUE);
	jhi_load_image_with_transparent_color(&but_down_red, BUTTON_SET_RED_DOWN, BLUE);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);
	jhi_load_font(&f2, MONSTER_FONT, 40);
	jhi_load_font(&f1, MONSTER_FONT, 30);
	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_set_fps_timer(32);

	jhi_init_text(&level);
	jhi_init_text(&level_name);
	jhi_set_text(&f1, &level, BLACK, "Dificuldade:");
	jhi_set_text(&f1, &level_name, RED, levels[get_rank_game_level()]);
	text_pointer[0] = &level;
	text_pointer_name[0] = &level_name;

	vet_arrow_type[0] = RANK_LEVEL;

	for (i = 0; i < num_of_ops; i++)
	{
		but_up_pointer[i] = &but_up;
		but_down_pointer[i] = &but_down;
	}


	jhi_init_mouse(&mouse);
	init_button(&back_title[0], &f1, "Retornar", BUTTON_TYPE_1);

	back_title[0].pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), jhi_get_image_width(&(back_title[0].img)),
			jhi_get_image_height(&(back_title[0].img))).x - 250;

	init_arrow_pos = jhi_get_point(jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&but_up), 0).x - 120, 20);

	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		jhi_draw_image(&back, jhi_get_point(0, 0));
		draw_rank(&f2, get_rank_game_level());
		draw_button(&back_title[0], jhi_get_point(back_title[0].pos.x, 500));
		draw_arrows(but_up_pointer, but_down_pointer, num_of_ops,
				 init_arrow_pos, text_pointer, text_pointer_name);


		jhi_init_mouse(&mouse);

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);
		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{

			HANDLE_ARROWS;
			pos = check_buttons(mouse, back_title, num_of_buttons);
			if (pos == 0) {te = IS_TITLE; jhi_play_effect(&ef1, 2000); break;}
		}

		jhi_wait_time();
	}

	jhi_free_font(&f1);
	jhi_free_font(&f2);
	jhi_free_image(&back);
	free_button(&back_title[0]);
	jhi_free_effect(&ef1);
	jhi_free_image(&but_up);
	jhi_free_image (&but_down);
	jhi_free_image(&but_up_red);
	jhi_free_image(&but_down_red);
	jhi_free_text(&level);
	jhi_free_text(&level_name);

	return te;
}
