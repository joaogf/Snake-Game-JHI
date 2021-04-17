#include "game.h"

TypeScreen game_play_scenario()
{
	Snake sn;
	JHI_Image wall;
	JHI_Image back;
	JHI_Text clock_text;
	JHI_Text paused_text;
	int i,colid_value,paused=0;
	int cnt_time = 0;
	int clock_time = 600 - 10 * get_frames_per_seconds();
	int clock_time_cur = 0;
	char clock_str[4];
	char player_info[30];
	JHI_KeyboardSt key_st;
	JHI_JoystickSt joy;
	Fruit fr;
	JHI_Effect eat_ef;
	JHI_Effect colid_ef;
	JHI_Font f1;
	TypeScreen texit = IS_CLOSE;
	JHI_Music mus;
	Player *player_pointer = get_player_by_index(0);

	jhi_load_image_with_transparent_color(&wall, WALL_FILE_PATH, BLUE);
	jhi_load_image(&back, BACK_FILE_PATH);
	wall.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), SCENARIO_WIDTH, SCENARIO_HEIGHT);
	jhi_load_effect(&eat_ef, EAT_SOUND);
	jhi_load_effect(&colid_ef, COLID_SOUND);
	jhi_load_music(&mus, MUSIC_PATH);
	jhi_load_font(&f1, MONSTER_FONT, 30);
	jhi_init_text(&clock_text);
	jhi_init_text(&paused_text);
	memset(clock_str, 0, sizeof(clock_str));
	sprintf(clock_str, "%d", clock_time);
	jhi_set_text(&f1, &clock_text, GREEN, clock_str);
	jhi_set_text(&f1, &paused_text, RED, "PAUSADO");

	init_snake(&sn, 0, jhi_get_point(wall.pos.x + BLOCK_LENGTH, wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), RED);
	init_fruit(&fr);

	jhi_init_joystick(&joy);

	jhi_set_fps_timer(get_frames_per_seconds());
	set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

	jhi_play_music(&mus, -1);

	player_pointer->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer->txt)), jhi_get_text_height(&(player_pointer->txt))).x;

	jhi_open_joystick_index(0);

	paused_text.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(paused_text)), jhi_get_text_height(&(paused_text)));


	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();

		jhi_update();
		jhi_clean();

		key_st.key = NO_KEY;
		jhi_init_joystick(&joy);

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			if(jhi_get_keyboard_status(i).key_event == KEYBOARD_DOWN)
			{
				key_st = jhi_get_keyboard_status(i);

			}

			if (jhi_get_joystick_status(i).joy_event == JHI_JOY_AXIS_MOTION ||
					jhi_get_joystick_status(i).joy_event == JHI_JOY_BUTTON_DOWN)
			{
				joy = jhi_get_joystick_status(i);
			}

		}

		if (key_st.key == KEY_ENTER || (joy.button == JOY_BUTTON_9 && joy.button_state == 1))
		{
			paused = !paused;
		}


		if (!paused)
		{
			if(get_control_game_type() == KEYBOARD)
				EXEC_WITH_DEATH_CHECK(player_pointer, move_snake(&sn, key_st.key));
			else
				EXEC_WITH_DEATH_CHECK(player_pointer, move_snake(&sn, convert_joy_to_key(jhi_get_joystick_dir(&joy, joy.axis_index))));
		}
		else
		{
			jhi_draw_text(&paused_text, paused_text.pos);
			continue;
		}

		jhi_draw_image(&back, jhi_get_point(0,0));

		draw_player(player_pointer, player_pointer->txt.pos.x, 50);

		jhi_draw_rect(jhi_get_point(SCENARIO_WIDTH, 50), jhi_get_text_height(&clock_text), jhi_get_text_width(&clock_text), WHITE);
		jhi_draw_text(&clock_text, jhi_get_point(SCENARIO_WIDTH, 50));

		jhi_draw_image(&wall, wall.pos);

		draw_fruit(&fr);

		EXEC_WITH_DEATH_CHECK(player_pointer, draw_snake(&sn));

		colid_value = snake_check_all_colision(&sn, &wall, &fr);

		if (texit == IS_DEATH)
		{
			if(draw_snake_anim(&sn)) { break;}
			continue;
		}

		if (colid_value == COLID_FRUIT)
		{

			if (fr.flag)
			{
				snake_eat(&sn);
				cnt_time = 0;

				player_increment_points(player_pointer, &f1 , fr.points);
				change_fruit(&fr);
				set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);
				player_pointer->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
						jhi_get_text_width(&(player_pointer->txt)), jhi_get_text_height(&(player_pointer->txt))).x;
				jhi_play_effect(&eat_ef, 0);
			}
			else
			{
				jhi_play_effect(&colid_ef, 150);
				player_pointer->is_death = IS_DEATH;
				texit = IS_DEATH;
				jhi_stop_music();
			}
		}

		else if (colid_value == COLID_DEATH)
		{
			jhi_play_effect(&colid_ef, 150);
			texit = IS_DEATH;
			player_pointer->is_death = IS_DEATH;
			jhi_stop_music();
		}


		jhi_wait_time();
		if (!paused)
		{
			cnt_time++;
			clock_time_cur++;
		}

		if (cnt_time == get_frames_per_seconds() * 3)
		{
			cnt_time = 0;
			change_fruit(&fr);
			set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

		}

		if (clock_time_cur == get_frames_per_seconds())
		{
			clock_time--;
			memset(clock_str, 0, sizeof(clock_str));
			sprintf(clock_str, "%d", clock_time);
			jhi_set_text(&f1, &clock_text, GREEN, clock_str);
			clock_time_cur = 0;
		}

		if (clock_time == 0)
		{
			texit = IS_DEATH;
			jhi_stop_music();
		}

	}

	jhi_free_image(&wall);
	jhi_free_image(&back);
	jhi_free_effect(&eat_ef);
	jhi_free_effect(&colid_ef);
	free_fruit(&fr);
	jhi_free_font(&f1);
	jhi_free_music(&mus);
	jhi_free_text(&clock_text);
	jhi_free_joystick_index(0);

	if (texit == IS_CLOSE)
	{
		free_player(player_pointer);
		free_snake(&sn);
	}

	return texit;
}

TypeScreen game_play_scenario_with_two_players()
{
	Snake sn[2];
	JHI_Image wall;
	JHI_Image back;
	JHI_Text clock_text;
	JHI_Text paused_text;
	int i, j,colid_value,paused=0;
	int cnt_time = 0;
	int clock_time = 600 - 10 * get_frames_per_seconds();
	int clock_time_cur = 0;
	char clock_str[4];
	char player_info[30];
	JHI_KeyboardSt key_st;
	JHI_JoystickSt joy;
	Fruit fr;
	JHI_Effect eat_ef;
	JHI_Effect colid_ef;
	JHI_Font f1;
	TypeScreen texit = IS_CLOSE;
	JHI_Music mus;
	Player *player_pointer;
	Player *player_pointer0 = get_player_by_index(0);
	Player *player_pointer1 = get_player_by_index(1);


	jhi_load_image_with_transparent_color(&wall, WALL_FILE_PATH, BLUE);
	jhi_load_image(&back, BACK_FILE_PATH);
	wall.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), SCENARIO_WIDTH, SCENARIO_HEIGHT);
	jhi_load_effect(&eat_ef, EAT_SOUND);
	jhi_load_effect(&colid_ef, COLID_SOUND);
	jhi_load_music(&mus, MUSIC_PATH);
	jhi_load_font(&f1, MONSTER_FONT, 30);
	jhi_init_text(&clock_text);
	jhi_init_text(&paused_text);
	memset(clock_str, 0, sizeof(clock_str));
	sprintf(clock_str, "%d", clock_time);
	jhi_set_text(&f1, &clock_text, GREEN, clock_str);
	jhi_set_text(&f1, &paused_text, RED, "PAUSADO");

	init_snake(&sn[0], 0, jhi_get_point(wall.pos.x + BLOCK_LENGTH, wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), RED);
	init_snake(&sn[1], 0, jhi_get_point(wall.pos.x - 2*BLOCK_LENGTH +
			jhi_get_image_width(&wall), wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), GREEN);

	sn[1].dir = KEY_LEFT;

	init_fruit(&fr);

	jhi_init_joystick(&joy);

	jhi_set_fps_timer(get_frames_per_seconds());
	set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

	jhi_play_music(&mus, -1);

	player_pointer0->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer0->txt)), jhi_get_text_height(&(player_pointer0->txt))).x;

	player_pointer1->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer1->txt)), jhi_get_text_height(&(player_pointer1->txt))).x;

	jhi_open_joystick_index(0);

	paused_text.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(paused_text)), jhi_get_text_height(&(paused_text)));


	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		key_st.key = NO_KEY;
		jhi_init_joystick(&joy);

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			if(jhi_get_keyboard_status(i).key_event == KEYBOARD_DOWN)
			{
				key_st = jhi_get_keyboard_status(i);

			}

			if (jhi_get_joystick_status(i).joy_event == JHI_JOY_AXIS_MOTION ||
					jhi_get_joystick_status(i).joy_event == JHI_JOY_BUTTON_DOWN)
			{
				joy = jhi_get_joystick_status(i);
			}

		}

		if (key_st.key == KEY_ENTER || (joy.button == JOY_BUTTON_9 && joy.button_state == 1))
		{
			paused = !paused;
		}


		if (!paused)
		{
				EXEC_WITH_DEATH_CHECK(player_pointer0, move_snake(&sn[0], key_st.key));
				EXEC_WITH_DEATH_CHECK(player_pointer1, move_snake(&sn[1], convert_joy_to_key(jhi_get_joystick_dir(&joy, joy.axis_index))));
		}
		else
		{
			jhi_draw_text(&paused_text, paused_text.pos);
			continue;
		}

		jhi_draw_image(&back, jhi_get_point(0,0));

		draw_player(player_pointer0, player_pointer0->txt.pos.x, 50);
		draw_player(player_pointer1, player_pointer0->txt.pos.x, 5);

		jhi_draw_rect(jhi_get_point(SCENARIO_WIDTH, 50), jhi_get_text_height(&clock_text), jhi_get_text_width(&clock_text), WHITE);
		jhi_draw_text(&clock_text, jhi_get_point(SCENARIO_WIDTH, 50));

		jhi_draw_image(&wall, wall.pos);

		draw_fruit(&fr);

		EXEC_WITH_DEATH_CHECK(player_pointer0, draw_snake(&sn[0]));
		EXEC_WITH_DEATH_CHECK(player_pointer1, draw_snake(&sn[1]));

		for (i = 0; i < 2; i++)
		{

			player_pointer = get_player_by_index(i);

			if (!player_pointer->is_death)
			{

				colid_value = snake_check_all_colision(&sn[i], &wall, &fr);

				if (colid_value == COLID_FRUIT)
				{

					if (fr.flag)
					{
						snake_eat(&sn[i]);
						cnt_time = 0;
						player_increment_points(player_pointer, &f1 , fr.points);
						change_fruit(&fr);
						set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);
						player_pointer->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
								jhi_get_text_width(&(player_pointer->txt)), jhi_get_text_height(&(player_pointer->txt))).x;
						jhi_play_effect(&eat_ef, 0);
					}
					else
					{
						jhi_play_effect(&colid_ef, 150);
						player_pointer->is_death = IS_DEATH;
						if (player_pointer0->is_death && player_pointer1->is_death)
						{
							texit = IS_DEATH2;
							jhi_stop_music();
						}
					}
				}

				else if (colid_value == COLID_DEATH)
				{
					jhi_play_effect(&colid_ef, 150);
					player_pointer->is_death = IS_DEATH2;
					if (player_pointer0->is_death && player_pointer1->is_death)
					{
						texit = IS_DEATH2;
						jhi_stop_music();
					}
				}

				player_pointer = get_player_by_index(!i);

				if (!player_pointer->is_death)
				{

					colid_value = is_colid_with_another_snake(&sn[i], &sn[!i]);

					if (colid_value == SNAKE_HEAD_COLID)
					{
						texit = IS_DEATH2;
						player_pointer0->is_death = IS_DEATH;
						player_pointer1->is_death = IS_DEATH;
						jhi_play_effect(&colid_ef, 150);
						jhi_stop_music();
					}
					else if (colid_value)
					{
						jhi_play_effect(&colid_ef, 150);
						player_pointer = get_player_by_index(i);
						player_pointer->is_death = IS_DEATH;
						if (player_pointer0->is_death && player_pointer1->is_death)
						{
							texit = IS_DEATH2;
							jhi_stop_music();
						}
					}
				}
			}
		}

		i = 0;
		j = 0;

		if(player_pointer0->is_death) { i = draw_snake_anim(&sn[0]);}
		if (player_pointer1->is_death) { j = draw_snake_anim(&sn[1]);}
		if (i && j && texit == IS_DEATH2){ break;}

		jhi_wait_time();
		if (!paused)
		{
			cnt_time++;
			clock_time_cur++;
		}

		if (cnt_time == get_frames_per_seconds() * 3)
		{
			cnt_time = 0;
			change_fruit(&fr);
			set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

		}

		if (clock_time_cur == get_frames_per_seconds())
		{
			clock_time--;
			memset(clock_str, 0, sizeof(clock_str));
			sprintf(clock_str, "%d", clock_time);
			jhi_set_text(&f1, &clock_text, GREEN, clock_str);
			clock_time_cur = 0;
		}

		if (clock_time == 0)
		{
			texit = IS_DEATH2;
			jhi_stop_music();
		}

	}

	jhi_free_image(&wall);
	jhi_free_image(&back);
	jhi_free_effect(&eat_ef);
	jhi_free_effect(&colid_ef);
	free_fruit(&fr);
	jhi_free_font(&f1);
	jhi_free_music(&mus);
	jhi_free_text(&clock_text);
	jhi_free_joystick_index(0);

	if (texit == IS_CLOSE)
	{
		free_player(player_pointer0);
		free_player(player_pointer1);
		free_snake(&sn[0]);
		free_snake(&sn[1]);
	}

	return texit;
}

TypeScreen game_play_scenario_with_three_players()
{
	Snake sn[3];
	JHI_Image wall;
	JHI_Image back;
	JHI_Text clock_text;
	JHI_Text paused_text;
	int i, j, k, colid_value,paused=0;
	int cnt_time = 0;
	int clock_time = 600 - 10 * get_frames_per_seconds();
	int clock_time_cur = 0;
	char clock_str[4];
	char player_info[30];
	JHI_KeyboardSt key_st;
	JHI_JoystickSt joy;
	Fruit fr;
	JHI_Effect eat_ef;
	JHI_Effect colid_ef;
	JHI_Font f1;
	TypeScreen texit = IS_CLOSE;
	JHI_Music mus;
	Player *player_pointer;
	Player *player_pointer0 = get_player_by_index(0);
	Player *player_pointer1 = get_player_by_index(1);
	Player *player_pointer2 = get_player_by_index(2);


	jhi_load_image_with_transparent_color(&wall, WALL_FILE_PATH, BLUE);
	jhi_load_image(&back, BACK_FILE_PATH);
	wall.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), SCENARIO_WIDTH, SCENARIO_HEIGHT);
	jhi_load_effect(&eat_ef, EAT_SOUND);
	jhi_load_effect(&colid_ef, COLID_SOUND);
	jhi_load_music(&mus, MUSIC_PATH);
	jhi_load_font(&f1, MONSTER_FONT, 30);
	jhi_init_text(&clock_text);
	jhi_init_text(&paused_text);
	memset(clock_str, 0, sizeof(clock_str));
	sprintf(clock_str, "%d", clock_time);
	jhi_set_text(&f1, &clock_text, GREEN, clock_str);
	jhi_set_text(&f1, &paused_text, RED, "PAUSADO");

	init_snake(&sn[0], 0, jhi_get_point(wall.pos.x + BLOCK_LENGTH, wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), RED);
	init_snake(&sn[1], 0, jhi_get_point(wall.pos.x - 2*BLOCK_LENGTH +
			jhi_get_image_width(&wall), wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), GREEN);
	init_snake(&sn[2], 0, jhi_get_point(wall.pos.x + BLOCK_LENGTH, 300),
				jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), WHITE);


	sn[1].dir = KEY_LEFT;
	sn[2].dir = KEY_UP;

	init_fruit(&fr);

	jhi_init_joystick(&joy);

	jhi_set_fps_timer(get_frames_per_seconds());
	set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

	jhi_play_music(&mus, -1);

	player_pointer0->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer0->txt)), jhi_get_text_height(&(player_pointer0->txt))).x;

	player_pointer1->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer1->txt)), jhi_get_text_height(&(player_pointer1->txt))).x;

	player_pointer2->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer2->txt)), jhi_get_text_height(&(player_pointer2->txt))).x;



	jhi_open_joystick_index(0);

	paused_text.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(paused_text)), jhi_get_text_height(&(paused_text)));


	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		key_st.key = NO_KEY;
		jhi_init_joystick(&joy);

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			if(jhi_get_keyboard_status(i).key_event == KEYBOARD_DOWN)
			{
				key_st = jhi_get_keyboard_status(i);

			}

			if (jhi_get_joystick_status(i).joy_event == JHI_JOY_AXIS_MOTION ||
					jhi_get_joystick_status(i).joy_event == JHI_JOY_BUTTON_DOWN)
			{
				joy = jhi_get_joystick_status(i);
			}

		}

		if (key_st.key == KEY_ENTER || (joy.button == JOY_BUTTON_9 && joy.button_state == 1))
		{
			paused = !paused;
		}


		if (!paused)
		{
				EXEC_WITH_DEATH_CHECK(player_pointer0, move_snake(&sn[0], key_st.key));
				EXEC_WITH_DEATH_CHECK(player_pointer1, move_snake(&sn[1], convert_joy_to_key(jhi_get_joystick_dir(&joy, joy.axis_index))));
				EXEC_WITH_DEATH_CHECK(player_pointer2, move_snake(&sn[2], get_snake_cpu_dir_two_snakes(SNAKETYPE2, &sn[0], &sn[1], &sn[2], &fr, &wall)));
		}
		else
		{
			jhi_draw_text(&paused_text, paused_text.pos);
			continue;
		}

		jhi_draw_image(&back, jhi_get_point(0,0));

		draw_player(player_pointer0, player_pointer0->txt.pos.x, 50);
		draw_player(player_pointer1, player_pointer0->txt.pos.x, 5);
		draw_player(player_pointer2, player_pointer0->txt.pos.x + jhi_get_text_width(&(player_pointer1->txt)), 5);



		jhi_draw_rect(jhi_get_point(SCENARIO_WIDTH, 50), jhi_get_text_height(&clock_text), jhi_get_text_width(&clock_text), WHITE);
		jhi_draw_text(&clock_text, jhi_get_point(SCENARIO_WIDTH, 50));

		jhi_draw_image(&wall, wall.pos);

		draw_fruit(&fr);

		EXEC_WITH_DEATH_CHECK(player_pointer0, draw_snake(&sn[0]));
		EXEC_WITH_DEATH_CHECK(player_pointer1, draw_snake(&sn[1]));
		EXEC_WITH_DEATH_CHECK(player_pointer2, draw_snake(&sn[2]));


		for (i = 0; i < 3; i++)
		{

			player_pointer = get_player_by_index(i);

			if (!player_pointer->is_death)
			{

				colid_value = snake_check_all_colision(&sn[i], &wall, &fr);

				if (colid_value == COLID_FRUIT)
				{

					if (fr.flag)
					{
						snake_eat(&sn[i]);
						cnt_time = 0;
						player_increment_points(player_pointer, &f1 , fr.points);
						change_fruit(&fr);
						set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);
						player_pointer->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
								jhi_get_text_width(&(player_pointer->txt)), jhi_get_text_height(&(player_pointer->txt))).x;
						jhi_play_effect(&eat_ef, 0);
					}
					else
					{
						jhi_play_effect(&colid_ef, 150);
						player_pointer->is_death = IS_DEATH;
						if (player_pointer0->is_death && player_pointer1->is_death && player_pointer2->is_death)
						{
							texit = IS_DEATH3;
							jhi_stop_music();
						}
					}
				}

				else if (colid_value == COLID_DEATH)
				{
					jhi_play_effect(&colid_ef, 150);
					player_pointer->is_death = IS_DEATH;
					if (player_pointer0->is_death && player_pointer1->is_death && player_pointer2->is_death)
					{
						texit = IS_DEATH3;
						jhi_stop_music();
					}
				}

				for (j = 0; j < 3; j++) {

					Player *p_pointer;

					p_pointer = get_player_by_index(j);

					if (p_pointer == player_pointer){continue;}

					if (!p_pointer->is_death)
					{

						colid_value = is_colid_with_another_snake(&sn[i], &sn[j]);

						if (colid_value == SNAKE_HEAD_COLID)
						{
							if (player_pointer0->is_death && player_pointer1->is_death && player_pointer2->is_death) {
								texit = IS_DEATH3;
								jhi_stop_music();
							}
							else {
								player_pointer->is_death = IS_DEATH;
								p_pointer->is_death = IS_DEATH;
								jhi_play_effect(&colid_ef, 150);
							}
						}
						else if (colid_value)
						{
							jhi_play_effect(&colid_ef, 150);
							player_pointer = get_player_by_index(i);
							player_pointer->is_death = IS_DEATH;
							if (player_pointer0->is_death && player_pointer1->is_death && player_pointer2->is_death)
							{
								texit = IS_DEATH3;
								jhi_stop_music();
							}
						}
					}
				}
			}
		}

		i = 0;
		j = 0;
		k = 0;

		if(player_pointer0->is_death) { i = draw_snake_anim(&sn[0]);}
		if (player_pointer1->is_death) { j = draw_snake_anim(&sn[1]);}
		if (player_pointer2->is_death) { k = draw_snake_anim(&sn[2]);}
		if (i && j && k && texit == IS_DEATH3){ break;}

		jhi_wait_time();
		if (!paused)
		{
			cnt_time++;
			clock_time_cur++;
		}

		if (cnt_time == get_frames_per_seconds() * 3)
		{
			cnt_time = 0;
			change_fruit(&fr);
			set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

		}

		if (clock_time_cur == get_frames_per_seconds())
		{
			clock_time--;
			memset(clock_str, 0, sizeof(clock_str));
			sprintf(clock_str, "%d", clock_time);
			jhi_set_text(&f1, &clock_text, GREEN, clock_str);
			clock_time_cur = 0;
		}

		if (clock_time == 0)
		{
			texit = IS_DEATH3;
			jhi_stop_music();
		}

	}

	jhi_free_image(&wall);
	jhi_free_image(&back);
	jhi_free_effect(&eat_ef);
	jhi_free_effect(&colid_ef);
	free_fruit(&fr);
	jhi_free_font(&f1);
	jhi_free_music(&mus);
	jhi_free_text(&clock_text);
	jhi_free_joystick_index(0);

	if (texit == IS_CLOSE)
	{
		free_player(player_pointer0);
		free_player(player_pointer1);
		free_player(player_pointer2);
		free_snake(&sn[0]);
		free_snake(&sn[1]);
		free_snake(&sn[2]);
	}

	return texit;
}

TypeScreen game_play_scenario_with_cpu()
{
	Snake sn[2];
	JHI_Image wall;
	JHI_Image back;
	JHI_Text clock_text;
	JHI_Text paused_text;
	int i, j,colid_value,paused=0;
	int cnt_time = 0;
	int clock_time = 600 - 10 * get_frames_per_seconds();
	int clock_time_cur = 0;
	char clock_str[4];
	char player_info[30];
	JHI_KeyboardSt key_st;
	JHI_JoystickSt joy;
	Fruit fr;
	JHI_Effect eat_ef;
	JHI_Effect colid_ef;
	JHI_Font f1;
	TypeScreen texit = IS_CLOSE;
	JHI_Music mus;
	Player *player_pointer;
	Player *player_pointer0 = get_player_by_index(0);
	Player *player_pointer1 = get_player_by_index(1);

	jhi_load_image_with_transparent_color(&wall, WALL_FILE_PATH, BLUE);
	jhi_load_image(&back, BACK_FILE_PATH);
	wall.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(), SCENARIO_WIDTH, SCENARIO_HEIGHT);
	jhi_load_effect(&eat_ef, EAT_SOUND);
	jhi_load_effect(&colid_ef, COLID_SOUND);
	jhi_load_music(&mus, MUSIC_PATH);
	jhi_load_font(&f1, MONSTER_FONT, 30);
	jhi_init_text(&clock_text);
	jhi_init_text(&paused_text);
	memset(clock_str, 0, sizeof(clock_str));
	sprintf(clock_str, "%d", clock_time);
	jhi_set_text(&f1, &clock_text, GREEN, clock_str);
	jhi_set_text(&f1, &paused_text, RED, "PAUSADO");

	init_snake(&sn[0], 0, jhi_get_point(wall.pos.x + BLOCK_LENGTH, wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), RED);
	init_snake(&sn[1], 0, jhi_get_point(wall.pos.x - 2*BLOCK_LENGTH +
			jhi_get_image_width(&wall), wall.pos.y + BLOCK_LENGTH),
			jhi_get_point(RECT_SNAKE_LENGHT,RECT_SNAKE_LENGHT), GREEN);

	sn[1].dir = KEY_LEFT;

	init_fruit(&fr);

	jhi_init_joystick(&joy);

	jhi_set_fps_timer(get_frames_per_seconds());
	//jhi_set_fps_timer(1);
	set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

	jhi_play_music(&mus, -1);

	player_pointer0->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer0->txt)), jhi_get_text_height(&(player_pointer0->txt))).x;

	player_pointer1->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(player_pointer1->txt)), jhi_get_text_height(&(player_pointer1->txt))).x;

	jhi_open_joystick_index(0);

	paused_text.pos = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
			jhi_get_text_width(&(paused_text)), jhi_get_text_height(&(paused_text)));


	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		if (texit == IS_DEATH2)
		{
			jhi_clean();
			jhi_update();
			break;
		}

		key_st.key = NO_KEY;
		jhi_init_joystick(&joy);

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			if(jhi_get_keyboard_status(i).key_event == KEYBOARD_DOWN)
			{
				key_st = jhi_get_keyboard_status(i);

			}

			if (jhi_get_joystick_status(i).joy_event == JHI_JOY_AXIS_MOTION ||
					jhi_get_joystick_status(i).joy_event == JHI_JOY_BUTTON_DOWN)
			{
				joy = jhi_get_joystick_status(i);
			}

		}

		if (key_st.key == KEY_ENTER || (joy.button == JOY_BUTTON_9 && joy.button_state == 1))
		{
			paused = !paused;
		}


		if (!paused)
		{
			if(get_control_game_type() == KEYBOARD)
				EXEC_WITH_DEATH_CHECK(player_pointer0, move_snake(&sn[0], key_st.key));
			else
				EXEC_WITH_DEATH_CHECK(player_pointer0, move_snake(&sn[0], convert_joy_to_key(jhi_get_joystick_dir(&joy, joy.axis_index))));

			EXEC_WITH_DEATH_CHECK(player_pointer1, move_snake(&sn[1], get_snake_cpu_dir(SNAKETYPE2, &sn[0], &sn[1], &fr, &wall)));
		}
		else
		{
			jhi_draw_text(&paused_text, paused_text.pos);
			continue;
		}

		jhi_draw_image(&back, jhi_get_point(0,0));

		draw_player(player_pointer0, player_pointer0->txt.pos.x, 50);
		draw_player(player_pointer1, player_pointer0->txt.pos.x, 5);

		jhi_draw_rect(jhi_get_point(SCENARIO_WIDTH, 50), jhi_get_text_height(&clock_text), jhi_get_text_width(&clock_text), WHITE);
		jhi_draw_text(&clock_text, jhi_get_point(SCENARIO_WIDTH, 50));

		jhi_draw_image(&wall, wall.pos);

		draw_fruit(&fr);

		EXEC_WITH_DEATH_CHECK(player_pointer0, draw_snake(&sn[0]));
		EXEC_WITH_DEATH_CHECK(player_pointer1, draw_snake(&sn[1]));

		for (i = 0; i < 2; i++)
		{

			player_pointer = get_player_by_index(i);

			if (!player_pointer->is_death)
			{

				colid_value = snake_check_all_colision(&sn[i], &wall, &fr);

				if (colid_value == COLID_FRUIT)
				{

					if (fr.flag)
					{
						snake_eat(&sn[i]);
						cnt_time = 0;
						player_increment_points(player_pointer, &f1 , fr.points);
						change_fruit(&fr);
						set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);
						player_pointer->txt.pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
								jhi_get_text_width(&(player_pointer->txt)), jhi_get_text_height(&(player_pointer->txt))).x;
						jhi_play_effect(&eat_ef, 0);
					}
					else
					{
						jhi_play_effect(&colid_ef, 150);
						player_pointer->is_death = IS_DEATH;
						if (player_pointer0->is_death && player_pointer1->is_death)
						{
							texit = IS_DEATH2;
							jhi_stop_music();
						}
					}
				}

				else if (colid_value == COLID_DEATH)
				{
					jhi_play_effect(&colid_ef, 150);
					player_pointer->is_death = IS_DEATH2;
					if (player_pointer0->is_death && player_pointer1->is_death)
					{
						texit = IS_DEATH2;
						jhi_stop_music();
					}
				}

				player_pointer = get_player_by_index(!i);

				if (!player_pointer->is_death)
				{

					colid_value = is_colid_with_another_snake(&sn[i], &sn[!i]);

					if (colid_value == SNAKE_HEAD_COLID)
					{
						player_pointer0->is_death = IS_DEATH;
						player_pointer1->is_death = IS_DEATH;
						texit = IS_DEATH2;
						jhi_stop_music();
					}
					else if (colid_value)
					{
						jhi_play_effect(&colid_ef, 150);
						player_pointer = get_player_by_index(i);
						player_pointer->is_death = IS_DEATH;
						if (player_pointer0->is_death && player_pointer1->is_death)
						{
							texit = IS_DEATH2;
							jhi_stop_music();
						}
					}
				}
			}
		}

		i = 0;
		j = 0;

		if(player_pointer0->is_death) { i = draw_snake_anim(&sn[0]);}
		if (player_pointer1->is_death) { j = draw_snake_anim(&sn[1]);}
		if (i && j && texit == IS_DEATH2){ break;}

		jhi_wait_time();
		if (!paused)
		{
			cnt_time++;
			clock_time_cur++;
		}

		if (cnt_time == get_frames_per_seconds() * 3)
		{
			cnt_time = 0;
			change_fruit(&fr);
			set_fruit_position(&fr, wall.pos, SCENARIO_WIDTH, SCENARIO_HEIGHT, BLOCK_LENGTH);

		}

		if (clock_time_cur == get_frames_per_seconds())
		{
			clock_time--;
			memset(clock_str, 0, sizeof(clock_str));
			sprintf(clock_str, "%d", clock_time);
			jhi_set_text(&f1, &clock_text, GREEN, clock_str);
			clock_time_cur = 0;
		}

		if (clock_time == 0)
		{
			texit = IS_DEATH2;
			jhi_stop_music();
		}

	}

	jhi_free_image(&wall);
	jhi_free_image(&back);
	jhi_free_effect(&eat_ef);
	jhi_free_effect(&colid_ef);
	free_fruit(&fr);
	jhi_free_font(&f1);
	jhi_free_music(&mus);
	jhi_free_text(&clock_text);
	jhi_free_joystick_index(0);

	if (texit == IS_CLOSE)
	{
		free_player(player_pointer0);
		free_player(player_pointer1);
		free_snake(&sn[0]);
		free_snake(&sn[1]);
	}

	return texit;
}
