GUI_ELEM* gui_text_(const char* txt, GUI_TEXT_STYLE_ENUM txt_style)
{
	GUI_ELEM* elm = gui_button(txt);

	switch(txt_style)
	{
		case GUI_TEXT_STYLE_NORMAL: elm->style = GUI_STYLE_TEXT; break;
		case GUI_TEXT_STYLE_SMALL: elm->style = GUI_STYLE_TEXT_SMALL; break;
		case GUI_TEXT_STYLE_NORMAL_BORDER: elm->style = GUI_STYLE_TEXT_BORDER; break;
		case GUI_TEXT_STYLE_SMALL_BORDER: elm->style = GUI_STYLE_TEXT_SMALL_BORDER; break;
		default: elm->style = GUI_STYLE_TEXT; break;
	}

	elm->flags |= GUI_FLG_LOCKED;

	gui_button_calc_size(elm);

	return elm;
}

GUI_ELEM* gui_text_word_wrap_(const char* txt, GUI_TEXT_STYLE_ENUM txt_style, int max_width, int max_height)
{
	GUI_ELEM* elm = gui_button(txt);

	switch(txt_style)
	{
		case GUI_TEXT_STYLE_NORMAL: elm->style = GUI_STYLE_TEXT; break;
		case GUI_TEXT_STYLE_SMALL: elm->style = GUI_STYLE_TEXT_SMALL; break;
		case GUI_TEXT_STYLE_NORMAL_BORDER: elm->style = GUI_STYLE_TEXT_BORDER; break;
		case GUI_TEXT_STYLE_SMALL_BORDER: elm->style = GUI_STYLE_TEXT_SMALL_BORDER; break;
		default: elm->style = GUI_STYLE_TEXT; break;
	}

	elm->flags |= GUI_FLG_LOCKED;
	elm->flags |= GUI_FLG_WORD_WRAP;

	elm->size = gui_helper_txt_calc_content_size_word_wrap(txt, gui_style_get(elm->style), elm->gui_state, (p32) { max_width, max_height });

	elm->calc_size = gui_element_calc_size;

	return elm;
}
