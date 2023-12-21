typedef struct SCREENLOGIN_STATE
{
	GUI_DISPLAY* dsp;
	GUI_PLANE* pln;

	GUI_ELEM* elm_USER_NAME_RULES;
	GUI_ELEM* elm_NO_USER_OR_PASSWORD;
	GUI_ELEM* elm_INCORRECT_EMAIL;
	GUI_ELEM* elm_USERNAME_TAKEN;
	GUI_ELEM* elm_PASSWORD_RULES;
	GUI_ELEM* elm_EMAIL_RULES;

	GUI_ELEM* elm_LOG_IN;
	GUI_ELEM* elm_SIGN_UP;

	GUI_ELEM* elm_INPUT_USER_NAME;
	GUI_ELEM* elm_INPUT_PASSWORD;
	GUI_ELEM* elm_INPUT_PASSWORD_AGAIN;
	GUI_ELEM* elm_INPUT_EMAIL;

	GUI_ELEM* elm_BUTTON_LOGIN;
	GUI_ELEM* elm_BUTTON_REGISTER;

	GUI_ELEM* elm_grp_LOGIN;
	GUI_ELEM* elm_grp_PASSWORD;
	GUI_ELEM* elm_grp_PASSWORD_AGAIN;
	GUI_ELEM* elm_grp_EMAIL;

	bool is_init;
} SCREENLOGIN_STATE;

static SCREENLOGIN_STATE screenlogin_state = { 0 };

void screenlogin_init(void);
void screenlogin_update(void);
void screenlogin_exit(void);

void screenlogin_init(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;

	if(!state->is_init)
	{
		state->is_init = true;

		{
			state->dsp = gui_display(0);
			state->dsp->pos = (p32) { (game_scr_width() - GAME_GUI_WIDTH) / 2,(game_scr_height() - GAME_GUI_HEIGHT) / 2 };
		}

		{
			state->pln = gui_plane(-1, (p32) { GAME_GUI_WIDTH, GAME_GUI_HEIGHT });
			gui_display_add_plane(state->dsp, state->pln);
			state->pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;
		}

		gui_plane_add_elm(state->pln, state->elm_USER_NAME_RULES = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_USER_NAME_RULES))));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_NO_USER_OR_PASSWORD = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_NO_USER_OR_PASSWORD))));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_INCORRECT_EMAIL = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_INCORRECT_EMAIL))));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_USERNAME_TAKEN = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_USERNAME_TAKEN))));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_PASSWORD_RULES = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_PASSWORD_RULES))));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_EMAIL_RULES = gui_text_word_wrap(txt_str_cat("#18@", game_txt_get(TXT_ID_EMAIL_RULES))));
		gui_plane_add_new_line(state->pln);


		gui_plane_add_elm(state->pln, state->elm_LOG_IN = gui_text(game_txt_get(TXT_ID_LOG_IN)));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_SIGN_UP = gui_text(game_txt_get(TXT_ID_SIGN_UP)));
		gui_plane_add_new_line(state->pln);


		gui_plane_add_elm(state->pln, state->elm_grp_LOGIN = gui_group_combo(
			gui_text(txt_str_cat(game_txt_get(TXT_ID_USER_NAME), ":")),
			state->elm_INPUT_USER_NAME = gui_text_input("")
		));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_grp_PASSWORD = gui_group_combo(
			gui_text(txt_str_cat(game_txt_get(TXT_ID_PASSWORD), ":")),
			state->elm_INPUT_PASSWORD = gui_text_input_password("")
		));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_grp_PASSWORD_AGAIN = gui_group_combo(
			gui_text(txt_str_cat(game_txt_get(TXT_ID_PASSWORD_AGAIN), ":")),
			state->elm_INPUT_PASSWORD_AGAIN = gui_text_input_password("")
		));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_grp_EMAIL = gui_group_combo(
			gui_text(txt_str_cat(game_txt_get(TXT_ID_EMAIL), ":")),
			state->elm_INPUT_EMAIL = gui_text_input("")
		));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_BUTTON_LOGIN = gui_button(game_txt_get(TXT_ID_LOGIN)));
		gui_plane_add_elm(state->pln, state->elm_BUTTON_REGISTER = gui_button(game_txt_get(TXT_ID_REGISTER)));
		gui_plane_add_new_line(state->pln);

		gui_layout_tile(state->pln);
	}
}

void screenlogin_hide_all_messages(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;

	state->elm_USER_NAME_RULES->flags |= GUI_FLG_HIDDEN;
	state->elm_NO_USER_OR_PASSWORD->flags |= GUI_FLG_HIDDEN;
	state->elm_INCORRECT_EMAIL->flags |= GUI_FLG_HIDDEN;
	state->elm_USERNAME_TAKEN->flags |= GUI_FLG_HIDDEN;
	state->elm_PASSWORD_RULES->flags |= GUI_FLG_HIDDEN;
	state->elm_EMAIL_RULES->flags |= GUI_FLG_HIDDEN;
 }

void screenlogin_hide_all(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;

	screenlogin_hide_all_messages();

	state->elm_LOG_IN->flags |= GUI_FLG_HIDDEN;
	state->elm_SIGN_UP->flags |= GUI_FLG_HIDDEN;

	state->elm_grp_LOGIN->flags |= GUI_FLG_HIDDEN;
	state->elm_grp_PASSWORD->flags |= GUI_FLG_HIDDEN;
	state->elm_grp_PASSWORD_AGAIN->flags |= GUI_FLG_HIDDEN;
	state->elm_grp_EMAIL->flags |= GUI_FLG_HIDDEN;

	state->elm_BUTTON_LOGIN->flags |= GUI_FLG_HIDDEN;
	state->elm_BUTTON_REGISTER->flags |= GUI_FLG_HIDDEN;
}

void screeenlogin_show_log_in(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;

	screenlogin_hide_all();

	state->elm_LOG_IN->flags &= ~GUI_FLG_HIDDEN;

	state->elm_grp_LOGIN->flags &= ~GUI_FLG_HIDDEN;
	state->elm_grp_PASSWORD->flags &= ~GUI_FLG_HIDDEN;

	state->elm_BUTTON_LOGIN->flags &= ~GUI_FLG_HIDDEN;
	state->elm_BUTTON_REGISTER->flags &= ~GUI_FLG_HIDDEN;
}

void screeenlogin_show_sign_up(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;

	screenlogin_hide_all();

	state->elm_SIGN_UP->flags &= ~GUI_FLG_HIDDEN;

	state->elm_grp_LOGIN->flags &= ~GUI_FLG_HIDDEN;
	state->elm_grp_PASSWORD->flags &= ~GUI_FLG_HIDDEN;
	state->elm_grp_PASSWORD_AGAIN->flags &= ~GUI_FLG_HIDDEN;
	state->elm_grp_EMAIL->flags &= ~GUI_FLG_HIDDEN;

	state->elm_BUTTON_LOGIN->flags &= ~GUI_FLG_HIDDEN;
	state->elm_BUTTON_REGISTER->flags &= ~GUI_FLG_HIDDEN;
}

void screenlogin_enter(void)
{
	game_screen_exit();

	GAME_SCREEN_UPDATE = screenlogin_update;
	GAME_SCREEN_EXIT = screenlogin_exit;

	screenlogin_init();

	SCREENLOGIN_STATE* state = &screenlogin_state;

	screeenlogin_show_log_in();
	//screeenlogin_show_sign_up();
	gui_layout_tile(state->pln);

	gui_add_display(state->dsp);
}

void screenlogin_update(void)
{
	//----------------------------------------------------------------------------------
	rd2_rect_fill(0, 0, 1000, 1000, gui_color(PAL_SILVER));
	//----------------------------------------------------------------------------------

	if(hal_key_down(KEY_LEFT_CONTROL))
	{
		if(gui_kb_last_key() == KEY_A)
		{
			SCREENLOGIN_STATE* state = &screenlogin_state;
			state->elm_grp_PASSWORD->flags ^= GUI_FLG_HIDDEN | GUI_FLG_DISABLED;
			gui_layout_tile(state->pln);
		}

		if(gui_kb_last_key() == KEY_S)
		{
			kernel_lock();

			if(player_char->user_network_state == PLAYER_CHAR_NETWORK_STATE_CONNECTED)
			{
				NETPACKET_DATASET_256 req = { 0 };
				req.data[0] = 1;
				req.data[1] = 2;
				req.data[2] = 3;
				req.data[3] = 4;
				req.data[4] = 5;
				req.data[5] = 55;
				req.data_size = 5;

				NETPACKET_BLOB* blb = new_netpacket_blob(NETPACKET_DATASET_256_SIZE());
				NETPACKET_DATASET_256_WRITE(blb, &req);
				blb->pos = 0;
				c_vec_push(&player_char->vec_netblob_send, blb);
			}

			kernel_free();
		}
	}
}

void screenlogin_exit(void)
{
	SCREENLOGIN_STATE* state = &screenlogin_state;
	gui_remove_display(state->dsp);
}