void screenlogin_init(void)
{
	GUI_DISPLAY* dsp = gui_top_display();

	int32 w = game_scr_width(), h = game_scr_height();
	dsp->pos = (p32) { (game_scr_width() - w) / 2,(game_scr_height() - h) / 2 };

	GUI_PLANE* pln = gui_plane(-1, (p32) { w, h });
	gui_display_add_plane(dsp, pln);

	pln->style = GUI_STYLE_EMPTY;

	//pln->flags |= GUI_FLG_INPUTLOCK;
	//pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;
	//pln->flags |= GUI_FLG_UNSCROLLABLE;

	int start_y = 25;
	int start_x = 25;

	int cur_y = 25;
	int cur_x = 25;

	int add_y = 25;

	{
		GUI_ELEM* elm = gui_text(game_txt_get(TXT_ID_USER_NAME_FLD));
		elm->origin = (p32) { cur_x,cur_y };

		gui_plane_add_elm(pln, elm);	

		gui_button_calc_size(elm);
		cur_x += elm->size.x;
	}

	{
		GUI_ELEM* elm = gui_text_input("");
		elm->origin = (p32) { cur_x,cur_y };

		gui_plane_add_elm(pln, elm);
	}

}