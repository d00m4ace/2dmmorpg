GUI_ELEM* gui_space_size(p32 size)
{
	GUI_ELEM* elm = gui_element();

	elm->elem_id = -GUI_ELEM_SPACE;
	elm->elem_type = GUI_ELEM_SPACE;

	elm->size = size;

	elm->style = GUI_STYLE_EMPTY;

	elm->show = gui_space_show;

	return elm;
}

GUI_ELEM* gui_space(void)
{
	GUI_ELEM* elm = gui_space_size((p32) { GUI_SPACE_WIDTH, GUI_SPACE_HEIGHT });

	return elm;
}

#define GUI_SPACE_NEW_LINE_SIZE 3

GUI_ELEM* gui_space_new_line(void)
{
	GUI_ELEM* elm = gui_space_size((p32) { 0, 0 });

	elm->elem_id = -GUI_ELEM_SPACE_NEW_LINE;
	elm->elem_type = GUI_ELEM_SPACE_NEW_LINE;

	return elm;
}

void gui_space_show(GUI_ELEM* elm, p32 offs)
{
	//rd2_tile(elm->pos.x + offs.x, elm->pos.y + offs.y, elm->size.x, elm->size.y, ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255), ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255));
}
