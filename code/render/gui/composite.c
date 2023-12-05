//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite(void)
{
	GUI_ELEM* cmps = gui_group();
	GUI_GROUP* group = cmps->data;

	cmps->elem_type = GUI_ELEM_COMPOSITE;
	cmps->elem_id = -1;

	cmps->style = GUI_STYLE_EMPTY;

	cmps->calc_size = gui_composite_calc_size;

	cmps->flags = 0;

	return cmps;
}
//--------------------------------------------------------------------------------------
void gui_composite_add(GUI_ELEM* cmps, GUI_ELEM* elm)
{
	gui_group_add_elm(cmps, elm);
}
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_combo(const char* txt, const char* txt_tl, const char* txt_tr, const char* txt_bl, const char* txt_br)
{
	GUI_ELEM* cmps = gui_composite();

	gui_composite_add(cmps, gui_button(txt));

	if(txt_tl != NULL)
	{
		GUI_ELEM* elm = gui_button(txt_tl);
		elm->style = GUI_STYLE_EMPTY_TEXT_THIN;
		elm->content_halign = HALIGN_LEFT;
		elm->content_valign = VALIGN_TOP;
		elm->calc_size = gui_element_calc_size;
		gui_composite_add(cmps, elm);
	}

	if(txt_tr != NULL)
	{
		GUI_ELEM* elm = gui_button(txt_tr);
		elm->style = GUI_STYLE_EMPTY_TEXT_THIN;
		elm->content_halign = HALIGN_RIGHT;
		elm->content_valign = VALIGN_TOP;
		elm->calc_size = gui_element_calc_size;
		gui_composite_add(cmps, elm);
	}

	if(txt_bl != NULL)
	{
		GUI_ELEM* elm = gui_button(txt_bl);
		elm->style = GUI_STYLE_EMPTY_TEXT_THIN;
		elm->content_halign = HALIGN_LEFT;
		elm->content_valign = VALIGN_BOTTOM;
		elm->calc_size = gui_element_calc_size;
		gui_composite_add(cmps, elm);
	}

	if(txt_br != NULL)
	{
		GUI_ELEM* elm = gui_button(txt_br);
		elm->style = GUI_STYLE_EMPTY_TEXT_THIN;
		elm->content_halign = HALIGN_RIGHT;
		elm->content_valign = VALIGN_BOTTOM;
		elm->calc_size = gui_element_calc_size;
		gui_composite_add(cmps, elm);
	}

	return cmps;
}

void gui_composite_set_fixed_size(GUI_ELEM* cmps, p32 fixed_size)
{
	GUI_ELEM* elm = gui_composite_get_elm(cmps, 0);
	elm->size = fixed_size;
	elm->calc_size = gui_element_calc_size;
}
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_tr(const char* txt, const char* txt_tr, p32 fixed_size)
{
	GUI_ELEM* cmps = gui_composite_combo(txt, NULL, txt_tr, NULL, NULL);
	gui_composite_set_fixed_size(cmps, fixed_size);
	return cmps;
}

GUI_ELEM* gui_composite_br(const char* txt, const char* txt_br, p32 fixed_size)
{
	GUI_ELEM* cmps = gui_composite_combo(txt, NULL, NULL, NULL, txt_br);
	gui_composite_set_fixed_size(cmps, fixed_size);
	return cmps;
}

GUI_ELEM* gui_composite_tr_br(const char* txt, const char* txt_tr, const char* txt_br, p32 fixed_size)
{
	GUI_ELEM* cmps = gui_composite_combo(txt, NULL, txt_tr, NULL, txt_br);
	gui_composite_set_fixed_size(cmps, fixed_size);
	return cmps;
}
//--------------------------------------------------------------------------------------
void gui_composite_calc_size(GUI_ELEM* cmps)
{
	GUI_GROUP* group = cmps->data;
	gui_group_calc_size(cmps);
	gui_helper_elems_set_size(&group->vptr_elems, cmps->size);
}
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_get_elm(GUI_ELEM* cmps, int32 indx)
{
	return gui_group_get_elm(cmps, indx);
}

void gui_composite_set_txt(GUI_ELEM* cmps, int32 indx, const char* txt)
{
	GUI_ELEM* elm = gui_composite_get_elm(cmps, indx);
	gui_button_set_txt(elm, txt);
}
//--------------------------------------------------------------------------------------
