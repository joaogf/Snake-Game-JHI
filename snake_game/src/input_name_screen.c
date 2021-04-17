#include "game.h"

TypeScreen game_input_name_player_screen()
{
	JHI_Image back;
	Button input_name;
	Button char_input;
	Button char_input2;
	Button play[1];
	JHI_Effect ef1;
	const int num_of_buttons = 1;
	int cur_player = 0;
	JHI_MouseSt mouse;
	JHI_KeyboardSt key;
	TypeScreen te = IS_CLOSE;
	int i, pos;
	char flag = 'z' + 2;
	char cprint = 'z' + 2;
	char c = 'a';
	char ch[2];
	char name[20];
	char pname[10];
	JHI_Font f1, f2;
	Player *player_pointer;

	jhi_load_font(&f1, REGULAR_FONT,30);
	jhi_load_font(&f2, MONSTER_FONT, 30);
	jhi_load_image(&back, BACK_FILE_PATH);
	jhi_load_effect(&ef1, CLICK_BOTTON_SOUND);

	jhi_set_fps_timer(32);

	memset(name, 0, sizeof(name));
	memset(pname, 0, sizeof(pname));
	strncpy(name, "Seu Nome ", sizeof(name));

	init_button(&char_input, &f1, "a", BUTTON_TYPE_1);
	init_button(&char_input2, &f1, "a", BUTTON_TYPE_2);
	init_button(&input_name, &f1, name, BUTTON_TYPE_1);
	init_button(&play[0], &f1, "Jogar", BUTTON_TYPE_1);

	play[0].pos.x = jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
				jhi_get_image_width(&(play[0].img)), jhi_get_image_height(&(play[0].img))).x;

	while(jhi_get_close_window() != JHI_CLOSE)
	{
		jhi_timer_start();
		jhi_update();
		jhi_clean();

		char_input.pos.x = 100;
		char_input.pos.y = 200;
		jhi_draw_image(&back, jhi_get_point(0, 0));
		draw_button(&play[0], jhi_get_point(play[0].pos.x, 500));
		mouse.mouse_event = MOUSE_NOT_EVENT;
		key.key_event = KEYBOARD_NOT_EVENT;

		for (i = 0; i < jhi_get_number_of_events(); i++)
		{
			mouse = jhi_get_mouse_status(i);
			key = jhi_get_keyboard_status(i);

		}

		if (mouse.mouse_event != MOUSE_NOT_EVENT)
		{
			cprint = check_input(&char_input, mouse, &flag, f1.length);

			pos = check_buttons(mouse, play, num_of_buttons);

			if (pos == 0 && strlen(pname) > 0) {
				player_pointer = get_player_by_index(cur_player);
				init_player(player_pointer, &f2, pname, 0);
				jhi_play_effect(&ef1, 2000);

				if (get_number_of_players() == -1)
				{
					player_pointer = get_player_by_index(1);
					init_player(player_pointer, &f2, "CPU", 0);
					te = IS_PLAY3;
					break;
				}

				if (cur_player + 1 == get_number_of_players() && get_number_of_players() == 1)
				{
					te = IS_PLAY;
					break;
				}
				if (cur_player + 1 == get_number_of_players() && get_number_of_players() == 2)
				{
					te = IS_PLAY2;
					break;
				}
				if (cur_player + 1 == get_number_of_players()-1 && get_number_of_players() == 3)
				{
					te = IS_PLAY4;
					player_pointer = get_player_by_index(2);
					init_player(player_pointer, &f2, "CPU", 0);
					break;
				}
				memset(name, 0, sizeof(name));
				memset(pname, 0, sizeof(pname));
				strncpy(name, "Seu Nome ", sizeof(name));
				cur_player++;
				free_button(&input_name);
				init_button(&input_name, &f1, name, BUTTON_TYPE_1);
			}

		}

		change_button_text(&input_name, name);

		set_button_size(&input_name, strlen(name)*jhi_get_image_width(&(char_input.img)),
					jhi_get_image_height(&(char_input.img)));

		draw_button(&input_name, jhi_get_point(jhi_get_central_pos(jhi_get_width_window(), jhi_get_height_window(),
				jhi_get_image_width(&(input_name.img)), jhi_get_image_height(&(input_name.img))).x, 100));

		while(c != 'z' + 1)
		{

			memset(ch, 0, sizeof(ch));
			memcpy(ch, &c, sizeof(c));

			if (c == flag)
			{
				change_button_text(&char_input2, ch);
				draw_button(&char_input2, char_input.pos);
			}
			else
			{
				change_button_text(&char_input, ch);
				draw_button(&char_input, char_input.pos);
			}

			if (c == cprint)
			{

				if (strlen(name) <= 19)
				{
					strncat(name, &c, sizeof(char));
					strncat(pname, &c, sizeof(char));
				}
				cprint = 'z' + 2;
			}

			char_input.pos.x += 3 * f1.length;


			if (c == 'h' || c == 'p' || c =='x')
			{
				char_input.pos.x = 100;
				char_input.pos.y += 3 * f1.length;
			}

			c++;

		}

		c = 'a';

		jhi_wait_time();
	}

	jhi_stop_music();
	jhi_free_image(&back);
	free_button(&input_name);
	free_button(&char_input);
	free_button(&char_input2);
	free_button(&play[0]);
	jhi_free_effect(&ef1);
	free_title_music();
	jhi_free_font(&f1);
	jhi_free_font(&f2);

	if (te == IS_CLOSE)
	{
		player_pointer = get_player_by_index(0);
		free_player(player_pointer);
	}

	return te;

}
