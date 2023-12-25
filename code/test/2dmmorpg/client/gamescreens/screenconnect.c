typedef struct SCREENCONNECT_STATE
{
	GUI_DISPLAY* dsp;
	GUI_PLANE* pln;

	GUI_ELEM* elm_CONNECTING;
	GUI_ELEM* elm_CONNECTING_DOTS;
	GUI_ELEM* elm_CONNECTED;
	GUI_ELEM* elm_DISCONNECTED;
	GUI_ELEM* elm_CONNECTION_ERROR;

	int32 time_now;

	bool is_init;
} SCREENCONNECT_STATE;

static SCREENCONNECT_STATE screenconnect_state = { 0 };

void screenconnect_init(void);
void screenconnect_update(void);
void screenconnect_exit(void);

void screenconnect_init(void)
{
	SCREENCONNECT_STATE* state = &screenconnect_state;

	if(!state->is_init)
	{
		NETWORKER* clnt_networker = clnt_pchar_networker_get();
		player_char = ((PCHAR_NETWORKER*)clnt_networker->worker_data)->user_data;
		tcpclient_connect(&client_netsession, clnt_networker, SERVER_IP_ADDRESS, SERVER_PORT);
	}

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

		gui_plane_add_elm(state->pln, state->elm_CONNECTING = gui_text_word_wrap(game_txt_get(TXT_ID_CONNECTING)));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_CONNECTING_DOTS = gui_text_word_wrap(("#!...")));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_CONNECTED = gui_text_word_wrap(game_txt_get(TXT_ID_CONNECTED)));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_DISCONNECTED = gui_text_word_wrap(game_txt_get(TXT_ID_DISCONNECTED)));
		gui_plane_add_new_line(state->pln);

		gui_plane_add_elm(state->pln, state->elm_CONNECTION_ERROR = gui_text_word_wrap(game_txt_get(TXT_ID_CONNECTION_ERROR)));
		gui_plane_add_new_line(state->pln);

		gui_layout_tile(state->pln);
	}
}

void screenconnect_hide_all_messages(void)
{
	SCREENCONNECT_STATE* state = &screenconnect_state;

	state->elm_CONNECTING->flags |= GUI_FLG_HIDDEN;
	state->elm_CONNECTING_DOTS->flags |= GUI_FLG_HIDDEN;
	state->elm_CONNECTED->flags |= GUI_FLG_HIDDEN;
	state->elm_DISCONNECTED->flags |= GUI_FLG_HIDDEN;
	state->elm_CONNECTION_ERROR->flags |= GUI_FLG_HIDDEN;
}

void screenconnect_enter(void)
{
	game_screen_exit();

	GAME_SCREEN_UPDATE = screenconnect_update;
	GAME_SCREEN_EXIT = screenconnect_exit;

	screenconnect_init();

	SCREENCONNECT_STATE* state = &screenconnect_state;

	screenconnect_hide_all_messages();

	state->elm_CONNECTING->flags &= ~GUI_FLG_HIDDEN;
	state->elm_CONNECTING_DOTS->flags &= ~GUI_FLG_HIDDEN;

	//screeenlogin_show_sign_up();
	gui_layout_tile(state->pln);

	gui_add_display(state->dsp);
}

void screenconnect_update(void)
{
	SCREENCONNECT_STATE* state = &screenconnect_state;

	if(player_char->user_network_state == PLAYER_CHAR_NETWORK_STATE_CONNECTED)
	{
		if(state->elm_CONNECTED->flags & GUI_FLG_HIDDEN)
		{
			state->elm_CONNECTING_DOTS->flags |= GUI_FLG_HIDDEN;
			state->elm_CONNECTED->flags &= ~GUI_FLG_HIDDEN;
			gui_layout_tile(state->pln);
		}
		else
		{
			if((hal_time_ms() - state->time_now) > 1000)
				screenlogin_enter();
		}
	}
	else if(player_char->user_network_state == PLAYER_CHAR_NETWORK_STATE_DISCONNECTED)
	{
		if(state->elm_CONNECTION_ERROR->flags & GUI_FLG_HIDDEN)
		{
			state->elm_CONNECTING_DOTS->flags |= GUI_FLG_HIDDEN;
			state->elm_CONNECTION_ERROR->flags &= ~GUI_FLG_HIDDEN;
			gui_layout_tile(state->pln);

			state->time_now = hal_time_ms();
		}
	}
}

void screenconnect_exit(void)
{
	SCREENCONNECT_STATE* state = &screenconnect_state;
	gui_remove_display(state->dsp);
}
