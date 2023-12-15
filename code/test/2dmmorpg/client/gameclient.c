//--------------------------------------------------------------------------------------
void (*GAME_SCREEN_UPDATE)(void);
void (*GAME_SCREEN_EXIT)(void);

void game_screen_update(void)
{
	if(GAME_SCREEN_UPDATE != NULL)
		GAME_SCREEN_UPDATE();
}

void game_screen_exit(void)
{
	if(GAME_SCREEN_EXIT != NULL)
		GAME_SCREEN_EXIT();

	GAME_SCREEN_UPDATE = NULL;
	GAME_SCREEN_EXIT = NULL;
}
//--------------------------------------------------------------------------------------
#include "gamescreens/screenlogin.c"
//--------------------------------------------------------------------------------------
int main(void)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();
	//--------------------------------------------------------------------------------------
	// GAME SCREEN INIT
	game_scr_init(GAME_SCR_WIDTH_MIN, GAME_SCR_HEIGHT_MIN, GAME_SCR_WIDTH_BORDER, GAME_SCR_HEIGHT_BORDER);
	//--------------------------------------------------------------------------------------
	GAME_SCREEN_UPDATE = NULL;
	GAME_SCREEN_EXIT = NULL;
	//--------------------------------------------------------------------------------------
	//game_txt_set_lang(TXT_LANG_ID_RU);
	//--------------------------------------------------------------------------------------
	screenlogin_enter();
	//--------------------------------------------------------------------------------------
	// Main game loop
	//--------------------------------------------------------------------------------------
	while(!hal_window_is_close())    // Detect window close button or ESC key
	{
		//----------------------------------------------------------------------------------
		// GAME UPDATE
		//----------------------------------------------------------------------------------
		game_screen_update();

		if(hal_key_down(KEY_LEFT_CONTROL))
		{
			if(gui_kb_last_key() == KEY_Q)
			{
				game_screen_exit();
			}

			if(gui_kb_last_key() == KEY_W)
			{
				screenlogin_enter();
			}
		}
		//----------------------------------------------------------------------------------
		// GUI UPDATE
		//----------------------------------------------------------------------------------
		gui_update();
		//----------------------------------------------------------------------------------
		// GAME SCREEN RENDER
		//----------------------------------------------------------------------------------
		if(hal_gesture_detected(GESTURE_DOUBLETAP))
		{
			GUI_ELEM* elm = gui_input_elm_pressed();

			if(elm != NULL && elm->pln)
				gui_plane_scroll_to(elm->pln, elm->pos);
		}
		//----------------------------------------------------------------------------------
		// GUI DRAW
		//----------------------------------------------------------------------------------
		gui_draw();
		//----------------------------------------------------------------------------------
		// Draw to screen
		//----------------------------------------------------------------------------------
		hal_draw_begin();

		hal_cls(ARGB32(0, 0, 0, 255));

		game_scr_draw();

		hal_draw_fps(20, 0);
		hal_draw_end();
		//----------------------------------------------------------------------------------
	}

	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	hal_system_shutdown();
	game_shutdown();
	return 0;
}
//--------------------------------------------------------------------------------------