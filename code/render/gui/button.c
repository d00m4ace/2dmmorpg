p32 gui_button_calc_content_size(GUI_ELEM* elm);

GUI_ELEM* gui_button(const char* txt)
{
	GUI_ELEM* elm = gui_element();
	GUI_BUTTON* btn = CALLOC(1, sizeof(GUI_BUTTON));

	elm->data = btn;

	elm->elem_type = GUI_ELEM_BUTTON;
	elm->elem_id = -1;

	elm->size = (p32) { 0,0 };

	elm->style = GUI_STYLE_BUTTON;

	elm->calc_size = gui_button_calc_size;
	elm->show = gui_button_show;

	elm->flags = 0;

	elm->content_halign = HALIGN_CENTER;
	elm->content_valign = VALIGN_MIDDLE;

	gui_button_set_txt(elm, txt);

	return elm;
}

GUI_ELEM* gui_button_fixed_size(const char* txt, p32 fixed_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->size = fixed_size;

	elm->calc_size = gui_element_calc_size;

	return elm;
}

GUI_ELEM* gui_button_size(const char* txt, p32 min_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->size = p32_max(gui_button_calc_content_size(elm), min_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}

GUI_ELEM* gui_button_size_word_wrap(const char* txt, p32 max_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, max_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}

GUI_ELEM* gui_button_style(const char* txt, GUI_STYLE_ENUM style)
{
	GUI_ELEM* elm = gui_button(txt);

	gui_element_set_style_and_size_min(elm, style);

	elm->calc_size = gui_element_calc_size;

	return elm;
}

void gui_button_set_txt(GUI_ELEM* elm, const char* txt)
{
	GUI_BUTTON* btn = elm->data;

	ASSERT((strlen(txt) + 1) < GUI_BUTTON_TXT_MAX_SIZE);

	strcpy(btn->txt, txt);

	elm->flags |= GUI_FLG_DIRT;
}

const char* gui_button_get_txt(GUI_ELEM* elm)
{
	GUI_BUTTON* btn = elm->data;
	return btn->txt;
}

p32 gui_button_calc_content_size(GUI_ELEM* elm)
{
	GUI_BUTTON* btn = elm->data;
	GUI_STYLE* style = gui_style_get(elm->style);

	p32 sz = gui_helper_txt_calc_content_size(btn->txt, style, elm->gui_state);

	sz.x += 3;

	return sz;
}

void gui_button_calc_size(GUI_ELEM* elm)
{
	if(elm->flags & GUI_FLG_DIRT)
	{
		elm->size = gui_button_calc_content_size(elm);
		gui_style_calc_size(elm);
	}

	elm->flags &= ~GUI_FLG_DIRT;
}

void gui_button_show(GUI_ELEM* elm, p32 offs)
{
	GUI_BUTTON* btn = elm->data;
	GUI_STYLE* style = gui_style_get(elm->style);

	int txt_len = strlen(btn->txt);

	rd2_scr_stack_push(elm->size.x, elm->size.y, gui_color(PAL_TRANSPARENT));

	gui_element_show(elm, (p32) { -elm->pos.x, -elm->pos.y });

	if(txt_len)
	{
		p32 cs;

		if(elm->flags & GUI_FLG_WORD_WRAP)
			cs = elm->size;
		else
		{
			cs = gui_button_calc_content_size(elm);
			cs = p32_min(cs, elm->size);
		}

		int32 halgn = gui_style_calc_halign(elm->content_halign, elm->size.x, cs.x);
		int32 valgn = gui_style_calc_valign(elm->content_valign, elm->size.y, cs.y);

		RD2FONT* fnt = game_font_get(style->font_id[elm->gui_state]);

		uint32 fnt_flgs = FNT_FLG_DRAW;

		if(elm->flags & GUI_FLG_WORD_WRAP)
			fnt_flgs |= FNT_FLG_WORD_WRAP;

		p32 offs = rd2_font_print(fnt, btn->txt, halgn + 2, valgn, 1, 0, gui_color(style->color[elm->gui_state]), fnt_flgs);
	}

	rd2_scr_stack_pop_show(p32_add(elm->pos, offs));
}
