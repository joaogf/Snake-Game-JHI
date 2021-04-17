#include "jhi_window.h"
#include "game.h"

int main()
{
	jhi_initialize_window(800, 600, 32, BLACK);
	jhi_choice_window_name("Snake Game");
	TypeScreen t = IS_INTRO;

	GAME_EXEC(t);

	jhi_quit_and_free();

	return 0;
}
