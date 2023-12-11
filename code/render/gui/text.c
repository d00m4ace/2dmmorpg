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
	GUI_ELEM* elm = gui_button_size_word_wrap(txt, max_size);

	elm->style = GUI_STYLE_TEXT;
	elm->flags |= GUI_FLG_LOCKED;

	return elm;
}