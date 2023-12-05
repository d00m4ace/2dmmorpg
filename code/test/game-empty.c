//--------------------------------------------------------------------------------------
int main(void)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();
	//--------------------------------------------------------------------------------------
	// low end android 1280x720
	//static int SCR_WIDTH_MIN = 320;
	//static int SCR_HEIGHT_MIN = 180;
	// hi end android 1080+
	static int SCR_WIDTH_MIN = 422;
	static int SCR_HEIGHT_MIN = 216;
	//--------------------------------------------------------------------------------------
	static int SCR_W_BORDER_MAX = 5;
	static int SCR_H_BORDER_MAX = 0;
#ifdef _WIN32	
	SCR_W_BORDER_MAX = 6;
	SCR_H_BORDER_MAX = 6;
#endif
	game_scr_init(SCR_WIDTH_MIN, SCR_HEIGHT_MIN, SCR_W_BORDER_MAX, SCR_H_BORDER_MAX);
	//--------------------------------------------------------------------------------------
	{
		GUI_DISPLAY* dsp = gui_display(0);
		gui_add_display(dsp);

		int32 w = 320, h = 180;
		dsp->pos = (p32){ (game_scr_width() - w) / 2,(game_scr_height() - h) / 2 };

		GUI_PLANE* pln = gui_plane(-1, (p32) { w, h });
		gui_display_add_plane(dsp, pln);

		pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;
		//pln->flags |= GUI_FLG_UNSCROLLABLE;
	}
	//--------------------------------------------------------------------------------------
	// Main game loop
	//--------------------------------------------------------------------------------------
	while(!hal_window_is_close())    // Detect window close button or ESC key
	{
		// Update
		//----------------------------------------------------------------------------------
		if(hal_key_pressed(KEY_ENTER) || hal_gesture_detected(GESTURE_TAP)) game_sound_play(GAME_SOUND_ID_BIPBIP);
		if(hal_key_pressed(KEY_SPACE) || hal_gesture_detected(GESTURE_DOUBLETAP)) game_sound_play(GAME_SOUND_ID_BIUBIU);

		if(hal_key_pressed_last() == KEY_PRINT_SCREEN)
		{
			hal_save_image(*rd2_scr_get(), "screenshot.png");
		}
		//----------------------------------------------------------------------------------
		rd2_rect_fill(0, 0, 1000, 1000, ARGB32(0, 200, 200, 255));
		//rd2_rect(0, 0, game_scr_width(), game_scr_height(), ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255));
		//----------------------------------------------------------------------------------		

		//----------------------------------------------------------------------------------
		// GUI UPDATE
		//----------------------------------------------------------------------------------
		gui_update();
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		// GAME SCREEN RENDER
		//----------------------------------------------------------------------------------
		if(hal_gesture_detected(GESTURE_DOUBLETAP))
		{
			GUI_ELEM* elm = gui_input_elm_pressed();

			if(elm != NULL && elm->pln)
				gui_plane_scroll_to(elm->pln, elm->pos);
		}

		rd2_box(8, 100, 8, 8, gui_color(PAL_AQUAMARINE), gui_color(PAL_BLUE));
		rd2_tile(8, 110, 8, 8, gui_color(PAL_AQUAMARINE), gui_color(PAL_BLUE));
		rd2_bubble(8, 120, 8, 8, gui_color(PAL_AQUAMARINE), gui_color(PAL_BLUE));
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		// GUI DRAW
		//----------------------------------------------------------------------------------
		gui_draw();
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		// Draw
		//----------------------------------------------------------------------------------
		hal_draw_begin();

		hal_cls(ARGB32(0, 0, 255, 255));
		//hal_cls(ARGB32(0, 0, 0, 255));

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
