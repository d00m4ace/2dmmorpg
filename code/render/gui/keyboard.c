//--------------------------------------------------------------------------------------
typedef struct GUI_KEYBOARD
{
	int32 key_pressed_last_repeat;
	int32 key_pressed_last_frames;

	int32 key_pressed_last;

	char char_pressed_last;

	int32 kb_map_indx;
	int32 kb_shift;

	GUI_ELEM* kb_from;

	bool kb_is_closed;
	bool kb_is_enter_pressed;
} GUI_KEYBOARD;
//--------------------------------------------------------------------------------------
static GUI_KEYBOARD gui_kb;
//--------------------------------------------------------------------------------------
static GUI_DISPLAY* gui_kb_dsp = NULL;
//--------------------------------------------------------------------------------------
#define TXT_I16_KB_KEYBOARD "#8&"
#define TXT_I16_KB_ENTER "#9&"
#define TXT_I16_KB_DELETE "#10&"
#define TXT_I16_KB_SHIFT "#11&"
#define TXT_I16_KB_ARROW_LEFT "#12&"
#define TXT_I16_KB_ARROW_RIGHT "#13&"
#define TXT_I16_KB_ARROW_UP "#14&"
#define TXT_I16_KB_ARROW_DOWN "#15&"

static const char* gui_kb_map[][50] =
{
	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
		"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
		"a", "s", "d", "f", "g", "h", "j", "k", "l", ";",
		"z", "x", "c", "v", "b", "n", "m", ",", ".", "?",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, " ", " ", " ", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},

	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
		"é", "ö", "ó", "ê", "å", "í", "ã", "ø", "ù", "ç",
		"ô", "û", "â", "à", "ï", "ð", "î", "ë", "ä", "æ",
		"ÿ", "÷", "ñ", "ì", "è", "ò", "ü", "á", "þ", "õ",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, "ú", "ý", "¸", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},

	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
		",", ".", "?", ":", "/", "*", "(", ")", "-", "+",
		"~", "@", "##", "$", "%", "^", "&", "[", "]", "\"",
		"'", "_", "!", "|", "{", "}", "\\", "<", ">", "=",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, ";", " ", " ", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},


	{
		",", ".", "?", ":", "/", "*", "(", ")", "-", "+",
		"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P",
		"A", "S", "D", "F", "G", "H", "J", "K", "L", "\"",
		"Z", "X", "C", "V", "B", "N", "M", "<", ">", "=",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, " ", " ", " ", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},

	{
		",", ".", "?", ":", "/", "*", "(", ")", "-", "+",
		"É", "Ö", "Ó", "Ê", "Å", "Í", "Ã", "Ø", "Ù", "Ç",
		"Ô", "Û", "Â", "À", "Ï", "Ð", "Î", "Ë", "Ä", "Æ",
		"ß", "×", "Ñ", "Ì", "È", "Ò", "Ü", "Á", "Þ", "Õ",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, "Ú", "Ý", "¨", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},

	{
		"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
		",", ".", "?", ":", "/", "*", "(", ")", "-", "+",
		"~", "@", "##", "$", "%", "^", "&", "[", "]", "\"",
		"'", "_", "!", "|", "{", "}", "\\", "<", ">", "=",
		TXT_I16_KB_SHIFT, TXT_I16_KB_KEYBOARD, ";", " ", " ", " ", TXT_I16_KB_DELETE, TXT_I16_KB_ARROW_LEFT, TXT_I16_KB_ARROW_RIGHT, TXT_I16_KB_ENTER,
	},
};

void gui_kb_toggle_keyboard(int32 indx)
{
	if(gui_kb_dsp != NULL)
	{
		GUI_PLANE* pln = gui_display_get_plane(gui_kb_dsp, 0);

		for(int32 i = 0; i < 50; i++)
		{
			GUI_ELEM* elm = gui_plane_get_elm(pln, i);
			gui_button_set_txt(elm, gui_kb_map[indx][i]);
		}
	}
}
//--------------------------------------------------------------------------------------
void gui_kb_from_set_txt(void)
{
	if(gui_kb.kb_from != NULL)
	{
		if(gui_kb.kb_from->elem_type == GUI_ELEM_TEXT_INPUT)
		{
			gui_text_input_set_txt(gui_kb.kb_from, gui_kb_text_input_get_txt());
		}
	}
}
//--------------------------------------------------------------------------------------
const char* gui_kb_text_input_get_txt(void)
{
	GUI_ELEM* kb_ti = gui_plane_get_elm(gui_display_get_plane(gui_kb_dsp, 0), 50);
	return gui_text_input_get_txt(kb_ti);
}

void gui_kb_text_input_set_txt(const char* txt)
{
	GUI_ELEM* kb_ti = gui_plane_get_elm(gui_display_get_plane(gui_kb_dsp, 0), 50);
	gui_text_input_set_txt(kb_ti, txt);
}

GUI_ELEM* gui_kb_get_from(void)
{
	return gui_kb.kb_from;
}

bool gui_kb_is_closed(void)
{
	return gui_kb.kb_is_closed;
}

bool gui_kb_is_enter_pressed(void)
{
	return gui_kb.kb_is_enter_pressed;
}
//--------------------------------------------------------------------------------------

void gui_kb_display_on_input(GUI_DISPLAY* dsp)
{
	gui_display_on_input(dsp);

	GUI_PLANE* pln = gui_display_get_plane(dsp, 0);
	GUI_ELEM* ti = gui_plane_get_elm(pln, 50);

	GUI_ELEM* elm_focused = gui_input_elm_focused();

	if(elm_focused != ti)
		gui_input_set_elm_focused(ti);

	bool is_closed = false;

	GUI_ELEM* elm_pressed = NULL;

	{
		int pressed_frames_repeat = 30;

		if(gui_input_pressed_frames() > 30)
			pressed_frames_repeat = 2;

		if(gui_input_pressed_frames() == 1 || (gui_input_pressed_frames() % pressed_frames_repeat) == 0)
			elm_pressed = gui_input_elm_pressed();
	}

	if(elm_pressed != NULL)
	{
		if(elm_pressed->elem_type == GUI_ELEM_BUTTON)
		{
			if(elm_pressed->elem_id == 40)
			{
				gui_kb.kb_shift = !gui_kb.kb_shift;
				gui_kb_toggle_keyboard(gui_kb.kb_shift * 3 + gui_kb.kb_map_indx);
			}
			else if(elm_pressed->elem_id == 41)
			{
				gui_kb.kb_map_indx = (gui_kb.kb_map_indx + 1) % 3;
				gui_kb_toggle_keyboard(gui_kb.kb_shift * 3 + gui_kb.kb_map_indx);
			}
			else if(elm_pressed->elem_id == 46)
			{
				gui_text_input_cmd_backspace(ti);
			}
			else if(elm_pressed->elem_id == 47)
			{
				gui_text_input_move_cursor(ti, -1);
			}
			else if(elm_pressed->elem_id == 48)
			{
				gui_text_input_move_cursor(ti, 1);
			}
			else if(elm_pressed->elem_id == 49)
			{
				gui_kb.kb_is_enter_pressed = true;
				is_closed = true;
			}
			else
			{
				gui_text_input_add_char(ti, gui_button_get_txt(elm_pressed)[0]);
			}
		}
	}

	if(gui_kb_last_key() == KEY_ENTER)
	{
		gui_kb.kb_is_enter_pressed = true;
		is_closed = true;
	}

	if((gui_input_is_pressed() || gui_input_is_released()) &&
		!aabb32_to_p32(gui_plane_get_aabb32(pln), gui_input_pos_start()))
		is_closed = true;

	if(is_closed)
	{
		if(gui_kb.kb_is_enter_pressed)
			gui_kb_from_set_txt();

		gui_remove_display(dsp);

		gui_kb.kb_is_closed = true;
	}
}

void gui_kb_text_input_on_input(GUI_ELEM* elm)
{
	gui_element_on_input(elm);

	if(elm == gui_input_elm_focused())
	{
		if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
		{
			char kb_last_char = gui_kb_last_char();

			if(kb_last_char)
				gui_text_input_add_char(elm, kb_last_char);
			else
			{
				GUI_TEXT_INPUT* ti = elm->data;

				if(gui_kb_last_key() == KEY_RIGHT)
					gui_text_input_move_cursor(elm, 1);

				if(gui_kb_last_key() == KEY_LEFT)
					gui_text_input_move_cursor(elm, -1);

				if(gui_kb_last_key() == KEY_DELETE)
					gui_text_input_cmd_delete(elm);

				if(gui_kb_last_key() == KEY_BACKSPACE)
					gui_text_input_cmd_backspace(elm);
			}
		}
	}
}

void gui_kb_show_keyboard(GUI_ELEM* elm)
{
	gui_kb.kb_map_indx = 0;
	gui_kb.kb_shift = 0;
	gui_kb.kb_from = elm;
	gui_kb.kb_is_closed = false;
	gui_kb.kb_is_enter_pressed = false;

	GUI_ELEM* kb_ti = gui_plane_get_elm(gui_display_get_plane(gui_kb_dsp, 0), 50);
	gui_text_input_set_txt(kb_ti, "");

	((GUI_TEXT_INPUT*)kb_ti->data)->flags = 0;

	if(gui_kb.kb_from != NULL)
	{
		if(gui_kb.kb_from->elem_type == GUI_ELEM_TEXT_INPUT)
		{
			gui_text_input_set_txt(kb_ti, gui_text_input_get_txt(gui_kb.kb_from));

			if(((GUI_TEXT_INPUT*)gui_kb.kb_from->data)->flags & GUI_TEXT_INPUT_FLG_PASSWORD)
				((GUI_TEXT_INPUT*)kb_ti->data)->flags |= GUI_TEXT_INPUT_FLG_PASSWORD;
		}
	}

	gui_kb_toggle_keyboard(0);
	gui_add_display(gui_kb_dsp);
}

//--------------------------------------------------------------------------------------
void gui_kb_init(void)
{
	if(gui_kb_dsp == NULL)
	{
		gui_kb_dsp = gui_display(-1);

		GUI_PLANE* pln = gui_plane(-1, (p32) { 320, 180 });

		gui_display_add_plane(gui_kb_dsp, pln);

		pln->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;

		pln->origin = (p32){ -gui_style_calc_halign(HALIGN_CENTER, pln->size.x, game_scr_width()) ,
							-gui_style_calc_valign(VALIGN_MIDDLE, pln->size.y, game_scr_height()) };

		for(int32 i = 0; i < 50; i++)
		{
			GUI_ELEM* btn = gui_button_style(gui_kb_map[0][i], GUI_STYLE_BUTTON_KEYBOARD);
			gui_plane_add_elm(pln, btn);
		}

		layout_tile_print(&pln->vptr_elems, pln->size.x, -1, (p32) { 1, 1 });

		{
			GUI_ELEM* ti = gui_text_input("");
			ti->flags = GUI_FLG_FOCUSABLE | GUI_FLG_MOUSELOCK;
			ti->size.x = pln->size.x;
			ti->origin = (p32){ 0,0 };

			ti->on_input = gui_kb_text_input_on_input;

			gui_helper_elems_add_origin(&pln->vptr_elems, (p32) { 0, ti->size.y + 1 });

			gui_plane_add_elm(pln, ti);
		}

		gui_kb_dsp->on_input = gui_kb_display_on_input;
	}
	else
	{
		GUI_PLANE* pln = gui_display_get_plane(gui_kb_dsp, 0);

		pln->origin = (p32){ -gui_style_calc_halign(HALIGN_CENTER, pln->size.x, game_scr_width()) ,
							-gui_style_calc_valign(VALIGN_MIDDLE, pln->size.y, game_scr_height()) };
	}

	gui_kb.kb_from = NULL;
	gui_kb.kb_is_closed = true;
	gui_kb.kb_is_enter_pressed = false;
}

void gui_kb_update(void)
{
	gui_kb.key_pressed_last = hal_key_pressed_last();

	if(gui_kb.key_pressed_last != 0)
	{
		gui_kb.key_pressed_last_repeat = gui_kb.key_pressed_last;
		gui_kb.key_pressed_last_frames = 0;
	}
	else if(gui_kb.key_pressed_last_repeat != 0)
	{
		if(hal_key_down(gui_kb.key_pressed_last_repeat))
		{
			gui_kb.key_pressed_last_frames++;

			int pressed_frames_repeat = 30;

			if(gui_kb.key_pressed_last_frames > 30)
				pressed_frames_repeat = 2;

			if((gui_kb.key_pressed_last_frames % pressed_frames_repeat) == 0)
				gui_kb.key_pressed_last = gui_kb.key_pressed_last_repeat;
		}
		else
		{
			gui_kb.key_pressed_last_repeat = 0;
			gui_kb.key_pressed_last_frames = 0;
		}
	}

	int ch = hal_char_pressed_last();

	if(ch >= 0x0430 && ch <= 0x044F)
		ch = 0xE0 + (ch - 0x0430);
	else if(ch >= 0x0410 && ch <= 0x042F)
		ch = 0xC0 + (ch - 0x0410);
	else if(ch == 0x0401)
		ch = 0xA8;
	else if(ch == 0x0451)
		ch = 0xB8;

	//if(ch > 255) ch = 0;

	gui_kb.char_pressed_last = ch;

	//if(ch != 0) PRINT("getch: %x %c", ch, gui_kb.char_pressed_last);
	//if(gui_kb.key_pressed_last != 0) PRINT("gui_kb_last_key: %x %d", gui_kb.key_pressed_last, gui_kb.key_pressed_last);
}

int gui_kb_last_key(void)
{
	return gui_kb.key_pressed_last;
}

char gui_kb_last_char(void)
{
	return gui_kb.char_pressed_last;
}
