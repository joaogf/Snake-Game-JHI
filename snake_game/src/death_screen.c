#include "game.h"


TypeScreen game_death_screen()
{
	JHI_Image back;
	JHI_Image snake_img;
	const int num_buttons = 3;
	Button vet_buttons[num_buttons];
	JHI_Effect ef1;
	JHI_Font f1,f2;
	JHI_MouseSt mouse;
	char pname[12];
	TypeScreen te = IS_CLOSE;
	int i,pos;
	Player *player_pointer = get_player_by_index(0);

	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_load_image_with_transparent_color(&snake_img, SNAKE_DEATH_FILE_PATH, BLACK);

	jhi_load_font(&f1, BUTTON_FONT, 20);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);
	jhi_load_font(&f2, MONSTER_FONT, 30);

	init_button(&vet_buttons[0], &f1, "Jogar Novamente", BUTTON_TYPE_1);
	init_button(&vet_buttons[1], &f1, "Voltar Tela Inicial", BUTTON_TYPE_1);
	init_button(&vet_buttons[2], &f1, "Sair", BUTTON_TYPE_1);

	snake_img.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&snake_img), jhi_get_image_height(&snake_img));



	for (i = 0; i < num_buttons; i++)
	{
		set_button_size(&vet_buttons[i], jhi_get_image_width(&(vet_buttons[1].img)), jhi_get_image_height(&(vet_buttons[1].img)));
	}

	vet_buttons[0].pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
					jhi_get_image_width(&(vet_buttons[0].img)), jhi_get_image_height(&(vet_buttons[0].img)));

	vet_buttons[0].pos.y = snake_img.pos.y + jhi_get_image_height(&snake_img);

	for (i = 1; i < num_buttons; i++)
	{
		vet_buttons[i].pos.x = vet_buttons[i-1].pos.x;
		vet_buttons[i].pos.y = vet_buttons[i-1].pos.y + jhi_get_image_height(&(vet_buttons[i-1].img));
	}



	jhi_set_fps_timer(32);


	add_rank(player_pointer->name, player_pointer->points, get_game_level());

	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		mouse.mouse_event = MOUSE_NOT_EVENT;

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);

		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{
			pos = check_buttons(mouse, vet_buttons, num_buttons);

			if (pos == 0) {te = IS_PLAY; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 1) {te = IS_TITLE; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 2) {te = IS_CLOSE; jhi_play_effect(&ef1, 2000); break;}
		}


		jhi_draw_image(&back, jhi_get_point(0, 0));
		draw_player(player_pointer, jhi_get_point(player_pointer->txt.pos.x, 50));
		jhi_draw_image(&snake_img, snake_img.pos);

		for (i = 0; i < num_buttons; i++)
		{
			draw_button(&vet_buttons[i], vet_buttons[i].pos);
		}

		jhi_wait_time();
	}

	jhi_free_image(&back);
	jhi_free_image(&snake_img);
	free_button(&vet_buttons[0]);
	free_button(&vet_buttons[1]);
	jhi_free_font(&f1);

	strncpy(pname, player_pointer->name, sizeof(pname));
	free_player(player_pointer);

	if (te != IS_CLOSE)
	{
		init_player(player_pointer, &f2, pname, 0);
	}

	jhi_free_font(&f2);

	return te;
}

TypeScreen game_death_screen2()
{
	JHI_Image back;
	JHI_Image snake_img, snake_img2;
	const int num_buttons = 3;
	Button vet_buttons[num_buttons];
	JHI_Effect ef1;
	JHI_Font f1,f2;
	JHI_MouseSt mouse;
	JHI_Text winner;
	char winner_txt[50];
	char buffer[30];
	char pname[12];
	char pname2[12];
	TypeScreen te = IS_CLOSE;
	int i,pos;
	Player *player_pointer;
	Player *player_pointer0 = get_player_by_index(0);
	Player *player_pointer1 = get_player_by_index(1);

	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_load_image_with_transparent_color(&snake_img, SNAKE_DEATH_FILE_PATH, BLACK);
	jhi_load_image_with_transparent_color(&snake_img2, SNAKE_DEATH_FILE_PATH2, BLACK);
	jhi_load_font(&f1, BUTTON_FONT, 20);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);
	jhi_load_font(&f2, MONSTER_FONT, 30);
	memset(winner_txt, 0, sizeof(char) * 30);

	if (player_pointer0->points == player_pointer1->points)
	{
		strcpy(winner_txt, "Empate");
	}
	else
	{
		pos = player_pointer1->points > player_pointer0->points;
		player_pointer = get_player_by_index(pos);
		player_pointer->cnt_victorys++;
		add_rank(player_pointer->name, player_pointer->points, get_game_level());
		strcpy(winner_txt, "Venceu: ");
		strcat(winner_txt, player_pointer->name);
		strcat(winner_txt, " Placar: ");
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", player_pointer->cnt_victorys);
		strncat(winner_txt, buffer, strlen(buffer));
		strcat(winner_txt, "x");
		memset(buffer, 0, sizeof(buffer));
		player_pointer = get_player_by_index(!pos);
		sprintf(buffer, "%d", player_pointer->cnt_victorys);
		strncat(winner_txt, buffer, strlen(buffer));
	}


	init_button(&vet_buttons[0], &f1, "Jogar Novamente", BUTTON_TYPE_1);
	init_button(&vet_buttons[1], &f1, "Voltar Tela Inicial", BUTTON_TYPE_1);
	init_button(&vet_buttons[2], &f1, "Sair", BUTTON_TYPE_1);

	snake_img.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&snake_img)*2, jhi_get_image_height(&snake_img));

	snake_img2.pos = snake_img.pos;

	snake_img2.pos.x += jhi_get_image_height(&snake_img);


	for (i = 0; i < num_buttons; i++)
	{
		set_button_size(&vet_buttons[i], jhi_get_image_width(&(vet_buttons[1].img)), jhi_get_image_height(&(vet_buttons[1].img)));
	}

	vet_buttons[0].pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
					jhi_get_image_width(&(vet_buttons[0].img)), jhi_get_image_height(&(vet_buttons[0].img)));

	vet_buttons[0].pos.y = snake_img.pos.y + jhi_get_image_height(&snake_img);

	for (i = 1; i < num_buttons; i++)
	{
		vet_buttons[i].pos.x = vet_buttons[i-1].pos.x;
		vet_buttons[i].pos.y = vet_buttons[i-1].pos.y + jhi_get_image_height(&(vet_buttons[i-1].img));
	}

	jhi_set_fps_timer(32);

	jhi_init_text(&winner);
	jhi_set_text(&f2, &winner, GREEN, winner_txt);

	winner.pos.x = snake_img.pos.x;//jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			//jhi_get_text_width(&winner), jhi_get_text_height(&winner)).x;

	winner.pos.y = 50;

	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		mouse.mouse_event = MOUSE_NOT_EVENT;

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);

		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{
			pos = check_buttons(mouse, vet_buttons, num_buttons);

			if (pos == 0) {if(get_number_of_players() == 2) te = IS_PLAY2; else te = IS_PLAY3; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 1) {te = IS_TITLE; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 2) {te = IS_CLOSE; jhi_play_effect(&ef1, 2000); break;}
		}


		jhi_draw_image(&back, jhi_get_point(0, 0));
		jhi_draw_image(&snake_img, snake_img.pos);
		jhi_draw_image(&snake_img2, snake_img2.pos);
		jhi_draw_text(&winner, winner.pos);

		for (i = 0; i < num_buttons; i++)
		{
			draw_button(&vet_buttons[i], vet_buttons[i].pos);
		}

		jhi_wait_time();
	}

	jhi_free_image(&back);
	jhi_free_image(&snake_img);
	jhi_free_image(&snake_img2);
	free_button(&vet_buttons[0]);
	free_button(&vet_buttons[1]);
	jhi_free_font(&f1);

	strncpy(pname, player_pointer0->name, sizeof(pname));
	strncpy(pname2, player_pointer1->name, sizeof(pname2));
	free_player(player_pointer0);
	free_player(player_pointer1);

	if (te != IS_CLOSE)
	{
		init_player(player_pointer0, &f2, pname, player_pointer0->cnt_victorys);
		init_player(player_pointer1, &f2, pname2, player_pointer1->cnt_victorys);
	}

	jhi_free_font(&f2);

	return te;
}

TypeScreen game_death_screen3()
{
	JHI_Image back;
	JHI_Image snake_img, snake_img2, snake_img3;
	const int num_buttons = 3;
	Button vet_buttons[num_buttons];
	JHI_Effect ef1;
	JHI_Font f1,f2;
	JHI_MouseSt mouse;
	JHI_Text winner;
	char winner_txt[50];
	char buffer[30];
	char pname[12];
	char pname2[12];
	char pname3[12];
	TypeScreen te = IS_CLOSE;
	int i,pos;
	Player *player_pointer;
	Player *player_pointer0 = get_player_by_index(0);
	Player *player_pointer1 = get_player_by_index(1);
	Player *player_pointer2 = get_player_by_index(2);

	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_load_image_with_transparent_color(&snake_img, SNAKE_DEATH_FILE_PATH, BLACK);
	jhi_load_image_with_transparent_color(&snake_img2, SNAKE_DEATH_FILE_PATH2, BLACK);
	jhi_load_image_with_transparent_color(&snake_img3, SNAKE_DEATH_FILE_PATH3, BLACK);
	jhi_load_font(&f1, BUTTON_FONT, 20);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);
	jhi_load_font(&f2, MONSTER_FONT, 30);
	memset(winner_txt, 0, sizeof(char) * 30);

	if (player_pointer0->points == player_pointer1->points &&
			player_pointer0->points == player_pointer2->points &&
			player_pointer1->points == player_pointer2->points)
	{
		strcpy(winner_txt, "Empate");
	}
	else
	{

		if (player_pointer0->points > player_pointer1->points
				&& player_pointer0->points > player_pointer2->points)
		{
			pos = 0;
		}
		else if (player_pointer1->points > player_pointer0->points
				&& player_pointer1->points > player_pointer2->points)
		{
			pos = 1;
		}
		else
		{
			pos = 2;
		}

		player_pointer = get_player_by_index(pos);
		player_pointer->cnt_victorys++;
		add_rank(player_pointer->name, player_pointer->points, get_game_level());
		strcpy(winner_txt, "Venceu: ");
		strcat(winner_txt, player_pointer->name);
		strcat(winner_txt, " Placar: ");
		memset(buffer, 0, sizeof(buffer));
		sprintf(buffer, "%d", player_pointer->cnt_victorys);
		strncat(winner_txt, buffer, strlen(buffer));
		strcat(winner_txt, "x");
		/*******************************/
		pos++;
		if (pos > 2){ pos = 0;}
		memset(buffer, 0, sizeof(buffer));
		player_pointer = get_player_by_index(pos);
		sprintf(buffer, "%d", player_pointer->cnt_victorys);
		strncat(winner_txt, buffer, strlen(buffer));
		strcat(winner_txt, "x");
		pos++;
		if (pos > 2){ pos = 0;}
		memset(buffer, 0, sizeof(buffer));
		player_pointer = get_player_by_index(pos);
		sprintf(buffer, "%d", player_pointer->cnt_victorys);
		strncat(winner_txt, buffer, strlen(buffer));
		/***********************************/
	}


	init_button(&vet_buttons[0], &f1, "Jogar Novamente", BUTTON_TYPE_1);
	init_button(&vet_buttons[1], &f1, "Voltar Tela Inicial", BUTTON_TYPE_1);
	init_button(&vet_buttons[2], &f1, "Sair", BUTTON_TYPE_1);

	snake_img.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_image_width(&snake_img)*2, jhi_get_image_height(&snake_img));

	snake_img2.pos = snake_img.pos;

	snake_img2.pos.x += jhi_get_image_height(&snake_img);

	snake_img3.pos = snake_img2.pos;

	snake_img3.pos.x += jhi_get_image_height(&snake_img2);


	for (i = 0; i < num_buttons; i++)
	{
		set_button_size(&vet_buttons[i], jhi_get_image_width(&(vet_buttons[1].img)), jhi_get_image_height(&(vet_buttons[1].img)));
	}

	vet_buttons[0].pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
					jhi_get_image_width(&(vet_buttons[0].img)), jhi_get_image_height(&(vet_buttons[0].img)));

	vet_buttons[0].pos.y = snake_img.pos.y + jhi_get_image_height(&snake_img);

	for (i = 1; i < num_buttons; i++)
	{
		vet_buttons[i].pos.x = vet_buttons[i-1].pos.x;
		vet_buttons[i].pos.y = vet_buttons[i-1].pos.y + jhi_get_image_height(&(vet_buttons[i-1].img));
	}
	jhi_set_fps_timer(32);

	jhi_init_text(&winner);
	jhi_set_text(&f2, &winner, GREEN, winner_txt);

	winner.pos.x = snake_img.pos.x;//jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			//jhi_get_text_width(&winner), jhi_get_text_height(&winner)).x;

	winner.pos.y = 50;

	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		mouse.mouse_event = MOUSE_NOT_EVENT;

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);

		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{
			pos = check_buttons(mouse, vet_buttons, num_buttons);

			if (pos == 0) {te = IS_PLAY4; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 1) {te = IS_TITLE; jhi_play_effect(&ef1, 2000); break;}
			else if (pos == 2) {te = IS_CLOSE; jhi_play_effect(&ef1, 2000); break;}
		}


		jhi_draw_image(&back, jhi_get_point(0, 0));
		jhi_draw_image(&snake_img, snake_img.pos);
		jhi_draw_image(&snake_img2, snake_img2.pos);
		jhi_draw_image(&snake_img3, snake_img3.pos);
		jhi_draw_text(&winner, winner.pos);

		for (i = 0; i < num_buttons; i++)
		{
			draw_button(&vet_buttons[i], vet_buttons[i].pos);
		}

		jhi_wait_time();
	}

	jhi_free_image(&back);
	jhi_free_image(&snake_img);
	jhi_free_image(&snake_img2);
	jhi_free_image(&snake_img3);
	free_button(&vet_buttons[0]);
	free_button(&vet_buttons[1]);
	jhi_free_font(&f1);

	strncpy(pname, player_pointer0->name, sizeof(pname));
	strncpy(pname2, player_pointer1->name, sizeof(pname2));
	strncpy(pname3, player_pointer2->name, sizeof(pname3));
	free_player(player_pointer0);
	free_player(player_pointer1);
	free_player(player_pointer2);

	if (te != IS_CLOSE)
	{
		init_player(player_pointer0, &f2, pname, player_pointer0->cnt_victorys);
		init_player(player_pointer1, &f2, pname2, player_pointer1->cnt_victorys);
		init_player(player_pointer2, &f2, pname3, player_pointer2->cnt_victorys);
	}

	jhi_free_font(&f2);

	return te;
}
