GUI_ELEM* gui_text(const char* txt)
{
	GUI_ELEM* elm = gui_button(txt);

	elm->style = GUI_STYLE_TEXT;
	elm->flags |= GUI_FLG_LOCKED;

	return elm;
}