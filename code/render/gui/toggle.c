//--------------------------------------------------------------------------------------
GUI_ELEM* gui_toggle(void)
{
	GUI_ELEM* tgl = gui_group();
	GUI_GROUP* group = tgl->data;

	tgl->elem_type = GUI_ELEM_TOGGLE;
	tgl->elem_id = -1;

	tgl->style = GUI_STYLE_EMPTY;

	tgl->on_input = gui_toggle_on_input;

	tgl->flags = 0;

	return tgl;
}
//--------------------------------------------------------------------------------------
GUI_ELEM* __gui_toggle_combo__(char* str0, ...)
{
	GUI_ELEM* tgl = gui_toggle();

	va_list args;
	va_start(args, str0);

	for(;;)
	{
		const char* txt = va_arg(args, const char*);

		if(txt == NULL)
			break;

		gui_toggle_add(tgl, gui_button(txt));
	}

	va_end(args);

	GUI_GROUP* group = tgl->data;
	gui_helper_elems_rest_origin(&group->vptr_elems);

	gui_toggle_set_selected(tgl, 0);

	return tgl;
}
//--------------------------------------------------------------------------------------
void gui_toggle_add(GUI_ELEM* tgl, GUI_ELEM* elm)
{
	gui_group_add_elm(tgl, elm);
}

GUI_ELEM* gui_toggle_get_elm(GUI_ELEM* tgl, int32 indx)
{
	return gui_group_get_elm(tgl, indx);
}
//--------------------------------------------------------------------------------------
int32 gui_toggle_elems_count(GUI_ELEM* tgl)
{
	return gui_group_elems_count(tgl);
}

int32 gui_toggle_get_selected(GUI_ELEM* tgl)
{
	GUI_GROUP* group = tgl->data;

	return gui_helper_elems_find_first_set_no_flags(&group->vptr_elems, GUI_FLG_HIDDEN);
}

void gui_toggle_set_selected(GUI_ELEM* tgl, int32 selected)
{
	GUI_GROUP* group = tgl->data;

	gui_helper_elems_add_flag(&group->vptr_elems, GUI_FLG_HIDDEN);

	GUI_ELEM* elm = gui_toggle_get_elm(tgl, selected);
	elm->flags &= ~GUI_FLG_HIDDEN;
}
//--------------------------------------------------------------------------------------
void gui_toggle_set_fixed_size(GUI_ELEM* tgl, p32 fixed_size)
{
	GUI_GROUP* group = tgl->data;

	gui_helper_elems_set_size(&group->vptr_elems, fixed_size);
	gui_helper_elems_set_calc_size(&group->vptr_elems, gui_element_calc_size);
}
//--------------------------------------------------------------------------------------
void gui_toggle_on_input(GUI_ELEM* tgl)
{
	gui_group_on_input(tgl);

	GUI_GROUP* group = tgl->data;

	if(group->elm_released != NULL)
	{
		int32 indx = gui_toggle_get_selected(tgl);

		if(indx != -1)
		{
			indx = (++indx) % gui_toggle_elems_count(tgl);
			gui_toggle_set_selected(tgl, indx);
		}		
	}
}
//--------------------------------------------------------------------------------------
