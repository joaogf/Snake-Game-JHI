#include "game.h"


TypeScreen game_intro_screen()
{
	JHI_Image logo;
	JHI_Font font;
	JHI_Text text;

	jhi_load_image(&logo, LOGO_LIB);
	jhi_init_text(&text);
	jhi_load_font(&font, BUTTON_FONT, 20);
	jhi_set_text(&font, &text , BLACK, "Powered by LIBJHI-SDL");

	TypeScreen te = IS_CLOSE;

	logo.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&logo), jhi_get_image_height(&logo));

	text.pos.x = logo.pos.x;
	text.pos.y = logo.pos.y + jhi_get_image_height(&logo);

	jhi_set_background_color(WHITE);

	int alpha = JHI_MIN_ALPHA;
	int inc = 3;


	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();
		jhi_set_alpha_image(&logo, alpha);
		jhi_set_alpha_text(&text, alpha);
		jhi_draw_image(&logo, logo.pos);
		jhi_draw_text(&text, text.pos);

		alpha+=inc;
		if (alpha >= JHI_MAX_ALPHA){
			jhi_transition_square_color(BLACK, DEFAULT_DIST, 15, 0);
			jhi_set_background_color(BLACK);
			jhi_update();
			jhi_clean();
			jhi_delay_mili_seconds(500);
			te = IS_TITLE;
			break;
		}


		jhi_wait_time();

	}


	jhi_free_image(&logo);
	jhi_free_text(&text);
	jhi_free_font(&font);
	return te;
}

TypeScreen game_title_screen()
{
	JHI_Image back;
	JHI_Image snake_img;
	Button vet_buttons[7];
	JHI_Effect ef1;
	JHI_Font f1,f2;
	const int num_buttons = 7;
	JHI_Text snake_text;
	JHI_MouseSt mouse;
	mouse.mouse_event = MOUSE_NOT_EVENT;
	TypeScreen te = IS_CLOSE;
	int i,pos;

	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_load_image_with_transparent_color(&snake_img, SNAKE_FILE_PATH, BLACK);
	jhi_load_font(&f1, BUTTON_FONT, 20);
	jhi_load_font(&f2, MONSTER_FONT, 80);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);
	load_music_title_screen();

	init_button(&vet_buttons[0], &f1, "Jogar Modo Pontos", BUTTON_TYPE_1);
	init_button(&vet_buttons[1], &f1, "Jogar Com CPU", BUTTON_TYPE_1);
	init_button(&vet_buttons[2], &f1, "Jogar Dois Jogadores", BUTTON_TYPE_1);
	init_button(&vet_buttons[3], &f1, "Jogar Dois Jogadores com CPU", BUTTON_TYPE_1);
	init_button(&vet_buttons[4], &f1, "Configuracoes", BUTTON_TYPE_1);
	init_button(&vet_buttons[5], &f1, "Rank", BUTTON_TYPE_1);
	init_button(&vet_buttons[6], &f1, "Sair", BUTTON_TYPE_1);
	jhi_init_text(&snake_text);

	snake_img.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&snake_img), jhi_get_image_height(&snake_img));

	snake_img.pos.y -= 100;

	for (i = 0; i < num_buttons; i++)
	{
		set_button_size(&vet_buttons[i], jhi_get_image_width(&(vet_buttons[3].img)), jhi_get_image_height(&(vet_buttons[3].img)));
	}

	jhi_set_text(&f2, &snake_text, BLACK, "Snake Game 1.3");

	snake_text.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), jhi_get_text_width(&snake_text),
			jhi_get_text_height(&snake_text)).x + 20;

	jhi_set_fps_timer(32);
	play_title_music();
	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);

		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{
			pos = check_buttons(mouse, vet_buttons, num_buttons);

			if (pos == 0) {te = IS_INPUT; set_number_of_players(1); jhi_play_effect(&ef1, 2000);  break;}
			else if (pos == 1) { te = IS_INPUT; set_number_of_players(-1); jhi_play_effect(&ef1, 2000);  break;}
			else if (pos == 2) { te = IS_INPUT; set_number_of_players(2); jhi_play_effect(&ef1, 2000);  break;}
			else if (pos == 3) { te = IS_INPUT; set_number_of_players(3); jhi_play_effect(&ef1, 2000);  break;}
			else if (pos == 4) {te = IS_CONF; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 5) {te = IS_RANK; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 6) {te = IS_CLOSE; jhi_play_effect(&ef1, 2000); break;}
		}


		jhi_draw_image(&back, jhi_get_point(0, 0));
		jhi_draw_text(&snake_text, jhi_get_point(snake_text.pos.x, 50));
		jhi_draw_image(&snake_img, snake_img.pos);
		draw_button(&vet_buttons[0], jhi_get_point(snake_img.pos.x - jhi_get_image_width(&snake_img),
				snake_img.pos.y + jhi_get_image_height(&snake_img) + jhi_get_image_height(&(vet_buttons[0].img))));
		for (i = 1; i < num_buttons; i++)
		{
			draw_button(&vet_buttons[i], jhi_get_point(snake_img.pos.x - jhi_get_image_width(&snake_img),
				vet_buttons[i-1].pos.y +  jhi_get_image_height(&(vet_buttons[0].img))));
		}

		jhi_wait_time();
	}

	jhi_free_image(&back);
	jhi_free_image(&snake_img);
	for (i = 0; i < num_buttons; i++)
	{
		free_button(&vet_buttons[i]);
	}
	jhi_free_font(&f1);
	jhi_free_font(&f2);
	jhi_free_effect(&ef1);
	jhi_free_text(&snake_text);

	return te;
}

TypeScreen game_get_close()
{
	return IS_CLOSE;
}
