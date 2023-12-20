GUI_ELEM* gui_text_input(const char* txt)
{
	GUI_ELEM* elm = gui_element();
	GUI_TEXT_INPUT* ti = CALLOC(1, sizeof(GUI_TEXT_INPUT));

	elm->data = ti;

	elm->elem_type = GUI_ELEM_TEXT_INPUT;
	elm->elem_id = -1;

	elm->style = GUI_STYLE_TEXT_INPUT;
	elm->size = gui_style_size_min(elm->style);

	elm->show = gui_text_input_show;
	elm->on_input = gui_text_input_on_input;
	elm->on_update = gui_text_input_on_update;

	elm->flags = 0;

	elm->content_halign = HALIGN_LEFT;
	elm->content_valign = VALIGN_MIDDLE;

	gui_text_input_set_txt(elm, txt);

	return elm;
}

GUI_ELEM* gui_text_input_password(const char* txt)
{
	GUI_ELEM* elm = gui_text_input(txt);
	GUI_TEXT_INPUT* ti = elm->data;

	ti->flags |= GUI_TEXT_INPUT_FLG_PASSWORD;

	return elm;
}

void gui_text_input_set_txt(GUI_ELEM* elm, const char* txt)
{
	GUI_TEXT_INPUT* ti = elm->data;

	ASSERT(strlen(txt) < GUI_TEXT_INPUT_TXT_MAX_SIZE);

	strcpy(ti->txt, txt);

	ti->cur_pos = strlen(txt);
	ti->x_offs = 0;

	elm->flags |= GUI_FLG_DIRT;
}

const char* gui_text_input_get_txt(GUI_ELEM* elm)
{
	GUI_TEXT_INPUT* ti = elm->data;
	return ti->txt;
}

p32 gui_text_input_calc_content_size(GUI_ELEM* elm, int32 cur_pos)
{
	GUI_TEXT_INPUT* ti = elm->data;
	GUI_STYLE* style = gui_style_get(elm->style);

	ASSERT(cur_pos < GUI_TEXT_INPUT_TXT_MAX_SIZE);

	char tmp_chr;

	if(cur_pos >= 0)
	{
		tmp_chr = ti->txt[cur_pos];
		ti->txt[cur_pos] = 0;
	}

	p32 sz = { 0 };

	if(ti->flags & GUI_TEXT_INPUT_FLG_PASSWORD)
	{
		static char tmp_txt[GUI_TEXT_INPUT_TXT_MAX_SIZE];
		int32 txt_len = strlen(ti->txt);
		memset(tmp_txt, '*', txt_len);
		tmp_txt[txt_len] = 0;
		sz = gui_helper_str_calc_content_size(tmp_txt, style, elm->gui_state);
	}
	else
		sz = gui_helper_str_calc_content_size(ti->txt, style, elm->gui_state);

	if(cur_pos >= 0)
		ti->txt[cur_pos] = tmp_chr;

	return sz;
}

void gui_text_input_move_cursor_to(GUI_ELEM* elm, int32 pos_x)
{
	GUI_TEXT_INPUT* ti = elm->data;

	int txt_len = strlen(ti->txt);

	int32 cur_pos = 0;

	while(cur_pos <= txt_len)
	{
		p32 ps = gui_text_input_calc_content_size(elm, cur_pos);

		if(ti->x_offs + ps.x > pos_x - elm->pos.x)
		{
			if(cur_pos > 0)
				cur_pos--;
			break;
		}

		cur_pos++;
	}

	if(cur_pos > txt_len)
		cur_pos = txt_len;

	ti->cur_pos = cur_pos;
}

void gui_text_input_show(GUI_ELEM* elm, p32 offs)
{
	GUI_TEXT_INPUT* ti = elm->data;
	GUI_STYLE* style = gui_style_get(elm->style);

	int txt_len = strlen(ti->txt);

	rd2_scr_stack_push(elm->size.x, elm->size.y, gui_color(PAL_TRANSPARENT));

	gui_element_show(elm, (p32) { -elm->pos.x, -elm->pos.y });

	{
		RD2FONT* fnt = game_font_get(style->font_id[elm->gui_state]);
		int32 fnt_height = fnt->char_height + fnt->padding_height;
		int32 valgn = gui_style_calc_valign(elm->content_valign, elm->size.y, fnt_height);

		p32 cur_pos = gui_text_input_calc_content_size(elm, ti->cur_pos);

		if(cur_pos.x + ti->x_offs > elm->size.x)
			ti->x_offs = elm->size.x - cur_pos.x - 3;

		if(cur_pos.x + ti->x_offs < 2)
			ti->x_offs = -cur_pos.x + 1;

		if(ti->flags & GUI_TEXT_INPUT_FLG_PASSWORD)
		{
			static char tmp_txt[GUI_TEXT_INPUT_TXT_MAX_SIZE];
			int32 txt_len = strlen(ti->txt);
			memset(tmp_txt, '*', txt_len);
			tmp_txt[txt_len] = 0;
			rd2_font_draw_str(fnt, tmp_txt, ti->x_offs, valgn, 1, 0, gui_color(style->color[elm->gui_state]));
		}
		else
			rd2_font_draw_str(fnt, ti->txt, ti->x_offs, valgn, 1, 0, gui_color(style->color[elm->gui_state]));

		if((elm == gui_input_elm_pressed() || elm == gui_input_elm_focused()) && (!hal_is_draw_flash_time() || ti->flags & GUI_TEXT_INPUT_FLG_SHOW_CURSOR))
			rd2_rect_fill(cur_pos.x + ti->x_offs, valgn - 2, 2, fnt_height + 4, gui_color(PAL_BLACK));
	}

	rd2_scr_stack_pop_show(p32_add(elm->pos, offs));

	ti->flags &= ~GUI_TEXT_INPUT_FLG_SHOW_CURSOR;
}

void gui_text_input_on_input(GUI_ELEM* elm)
{
	gui_element_on_input(elm);

	if(elm == gui_input_elm_released())
	{
		gui_kb_show_keyboard(elm);
		return;
	}
}

void gui_text_input_on_update(GUI_ELEM* elm)
{
	gui_element_on_update(elm);

	if(elm == gui_input_elm_pressed())
	{
		if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
		{
			gui_text_input_move_cursor_to(elm, gui_input_pos().x);
		}
	}
}

void gui_text_input_move_cursor(GUI_ELEM* elm, int32 offs)
{
	GUI_TEXT_INPUT* ti = elm->data;

	ti->cur_pos += offs;

	if(ti->cur_pos < 0)
		ti->cur_pos = 0;

	int txt_len = strlen(ti->txt);

	if(ti->cur_pos > txt_len)
		ti->cur_pos = txt_len;

	ti->flags |= GUI_TEXT_INPUT_FLG_SHOW_CURSOR;
}

void gui_text_input_add_char(GUI_ELEM* elm, char key)
{
	GUI_TEXT_INPUT* ti = elm->data;

	int txt_len = strlen(ti->txt);

	if(txt_len + 1 >= GUI_TEXT_INPUT_TXT_MAX_SIZE)
		return;

	ASSERT(txt_len < GUI_TEXT_INPUT_TXT_MAX_SIZE);

	if(ti->cur_pos < txt_len)
	{
		char tmp_txt[GUI_TEXT_INPUT_TXT_MAX_SIZE];

		strcpy(tmp_txt, ti->txt);
		tmp_txt[ti->cur_pos] = key;
		strcpy(tmp_txt + ti->cur_pos + 1, ti->txt + ti->cur_pos);

		strcpy(ti->txt, tmp_txt);
	}
	else
	{
		ti->txt[txt_len] = key;
		ti->txt[txt_len + 1] = 0;
	}

	ti->cur_pos++;

	ti->flags |= GUI_TEXT_INPUT_FLG_SHOW_CURSOR;
}

void gui_text_input_cmd_delete(GUI_ELEM* elm)
{
	GUI_TEXT_INPUT* ti = elm->data;

	char* str = ti->txt + ti->cur_pos;

	while(*str)
	{
		*str = *(str + 1);
		str++;
	}

	ti->flags |= GUI_TEXT_INPUT_FLG_SHOW_CURSOR;
}

void gui_text_input_cmd_backspace(GUI_ELEM* elm)
{
	GUI_TEXT_INPUT* ti = elm->data;

	if(ti->cur_pos > 0)
	{
		gui_text_input_move_cursor(elm, -1);
		gui_text_input_cmd_delete(elm);
	}

	ti->flags |= GUI_TEXT_INPUT_FLG_SHOW_CURSOR;
}
