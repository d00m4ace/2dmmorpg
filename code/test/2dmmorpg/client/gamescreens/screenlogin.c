void screenlogin_init(void)
{
	GUI_DISPLAY* dsp = gui_top_display();

	int32 w = 400, h = 200;
	dsp->pos = (p32) { (game_scr_width() - w) / 2,(game_scr_height() - h) / 2 };

	GUI_PLANE* pln = gui_plane(-1, (p32) { w, h });
	gui_display_add_plane(dsp, pln);

	pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;

	{
		GUI_ELEM* elm = gui_text_word_wrap(game_txt_get(TXT_ID_USER_NAME_RULES), (p32) { w, h });
		gui_plane_add_elm(pln, elm);
	}

	gui_plane_add_new_line(pln);

	{
		GUI_ELEM* elm = gui_text(txt_str_cat(game_txt_get(TXT_ID_USER_NAME), ":"));
		gui_plane_add_elm(pln, elm);
	}

	{
		GUI_ELEM* elm = gui_text_input("");
		gui_plane_add_elm(pln, elm);
	}

	gui_plane_add_new_line(pln);

	{
		GUI_ELEM* elm = gui_text(txt_str_cat(game_txt_get(TXT_ID_PASSWORD), ":"));
		gui_plane_add_elm(pln, elm);
	}

	{
		GUI_ELEM* elm = gui_text_input("");
		gui_plane_add_elm(pln, elm);
	}

	gui_plane_add_new_line(pln);

	{
		GUI_ELEM* elm = gui_button(game_txt_get(TXT_ID_LOGIN));
		gui_plane_add_elm(pln, elm);
	}

	gui_plane_add_space(pln);

	{
		GUI_ELEM* elm = gui_button(game_txt_get(TXT_ID_REGISTER));
		gui_plane_add_elm(pln, elm);
	}

	gui_plane_add_new_line(pln);

	gui_layout_tile(pln);
}