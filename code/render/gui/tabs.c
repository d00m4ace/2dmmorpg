//--------------------------------------------------------------------------------------
GUI_ELEM* gui_tabs(void)
{
	GUI_ELEM* tbs = gui_group();
	GUI_GROUP* group = tbs->data;

	tbs->elem_type = GUI_ELEM_TABS;
	tbs->elem_id = -1;

	tbs->style = GUI_STYLE_EMPTY;

	tbs->on_input = gui_tabs_on_input;

	tbs->flags = 0;

	return tbs;
}

void gui_tabs_add(GUI_ELEM* tbs, GUI_ELEM* elm)
{
	gui_group_add_elm(tbs, elm);
}

GUI_ELEM* gui_tabs_get_elm(GUI_ELEM* tbs, int32 indx)
{
	return gui_group_get_elm(tbs, indx);
}

GUI_ELEM* __gui_tabs_combo__(char* str0, ...)
{
	GUI_ELEM* tbs = gui_tabs();

	va_list args;
	va_start(args, str0);

	for(;;)
	{
		const char* txt = va_arg(args, const char*);

		if(txt == NULL)
			break;

		gui_tabs_add(tbs, gui_button(txt));
	}

	va_end(args);

	gui_tabs_align(tbs, GUI_ALIGN_HORIZONTAL);

	gui_tabs_set_selected(tbs, 0);

	return tbs;
}

void gui_tabs_align(GUI_ELEM* tbs, GUI_ALIGN_ENUM align)
{
	GUI_GROUP* group = tbs->data;
	VPTR* vptr_elems = &group->vptr_elems;

	if(align == GUI_ALIGN_VERTICAL)
		layout_tile_print(vptr_elems, -1, 1, (p32) { 1, 1 });
	else
		layout_tile_print(vptr_elems, -1, -1, (p32) { 1, 1 });
}

int32 gui_tabs_elems_count(GUI_ELEM* tbs)
{
	return gui_group_elems_count(tbs);
}

int32 gui_tabs_get_selected(GUI_ELEM* tbs)
{
	GUI_GROUP* group = tbs->data;

	return gui_helper_elems_find_first_set_flags(&group->vptr_elems, GUI_FLG_SELECTED);
}

void gui_tabs_set_selected(GUI_ELEM* tbs, int32 selected)
{
	GUI_GROUP* group = tbs->data;

	gui_helper_elems_rest_flag(&group->vptr_elems, GUI_FLG_SELECTED);

	GUI_ELEM* elm = gui_tabs_get_elm(tbs, selected);
	elm->flags |= GUI_FLG_SELECTED;
}

void gui_tabs_set_fixed_size(GUI_ELEM* tbs, p32 fixed_size)
{
	GUI_GROUP* group = tbs->data;

	gui_helper_elems_set_size(&group->vptr_elems, fixed_size);
	gui_helper_elems_set_calc_size(&group->vptr_elems, gui_element_calc_size);

	gui_tabs_align(tbs, GUI_ALIGN_HORIZONTAL);
}

void gui_tabs_on_input(GUI_ELEM* tbs)
{
	gui_group_on_input(tbs);

	GUI_GROUP* group = tbs->data;

	if(group->elm_released != NULL)
	{
		int32 indx = vptr_elm_indx(&group->vptr_elems, group->elm_released);

		if(indx != -1)
			gui_tabs_set_selected(tbs, indx);

		gui_helper_elems_rest_gui_state(&group->vptr_elems);
	}
}
