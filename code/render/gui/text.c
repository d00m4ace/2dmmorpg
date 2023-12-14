GUI_ELEM* gui_text(const char* txt)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT;
	elm->flags |= GUI_FLG_LOCKED;

	gui_button_calc_size(elm);

	return elm;
}

GUI_ELEM* gui_text_word_wrap(const char* txt, p32 max_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT;

	elm->flags |= GUI_FLG_LOCKED;
	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, max_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}

GUI_ELEM* gui_text_small(const char* txt)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_SMALL;
	elm->flags |= GUI_FLG_LOCKED;

	gui_button_calc_size(elm);

	return elm;
}

GUI_ELEM* gui_text_small_word_wrap(const char* txt, p32 max_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_SMALL;

	elm->flags |= GUI_FLG_LOCKED;
	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, max_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}


GUI_ELEM* gui_text_border(const char* txt)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_BORDER;
	elm->flags |= GUI_FLG_LOCKED;

	gui_button_calc_size(elm);

	return elm;
}

GUI_ELEM* gui_text_small_border(const char* txt)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_SMALL_BORDER;
	elm->flags |= GUI_FLG_LOCKED;

	gui_button_calc_size(elm);

	return elm;
}

GUI_ELEM* gui_text_border_word_wrap(const char* txt, p32 max_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_BORDER;

	elm->flags |= GUI_FLG_LOCKED;
	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, max_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}

GUI_ELEM* gui_text_small_border_word_wrap(const char* txt, p32 max_size)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT_SMALL_BORDER;

	elm->flags |= GUI_FLG_LOCKED;
	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, max_size);

	elm->calc_size = gui_element_calc_size;

	return elm;
}