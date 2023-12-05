//--------------------------------------------------------------------------------------
GUI_ELEM* gui_spinner(const char* txt, int32 val, int32 val_min, int32 val_max)
{
	GUI_ELEM* grp = gui_group();
	GUI_SPINNER* spinner = CALLOC(1, sizeof(GUI_SPINNER));

	GUI_GROUP* group = grp->data;
	group->data = spinner;

	grp->elem_type = GUI_ELEM_SPINNER;
	grp->elem_id = -1;

	grp->style = GUI_STYLE_EMPTY;// GUI_ELEM_PROGRESSBAR;

	grp->on_input = gui_spinner_on_input;

	grp->flags = 0;

	grp->flags |= GUI_FLG_MOUSELOCK;

	//grp->flags |= GUI_FLG_MOUSELOCK | GUI_FLG_LOCKED | GUI_FLG_DISABLED;

	spinner->progbar = gui_progressbar_fixed_size(txt, val, val_min, val_max, gui_style_size_min(GUI_STYLE_SPINNER));
	spinner->btn_dec = gui_button_fixed_size("<", gui_style_size_min(GUI_STYLE_SPINNER_BUTTON));
	spinner->btn_inc = gui_button_fixed_size(">", gui_style_size_min(GUI_STYLE_SPINNER_BUTTON));

	spinner->progbar->style = GUI_STYLE_SPINNER;
	spinner->btn_dec->style = spinner->btn_inc->style = GUI_STYLE_SPINNER_BUTTON;

	gui_group_add_elm(grp, spinner->btn_dec);
	gui_group_add_elm(grp, spinner->progbar);
	gui_group_add_elm(grp, spinner->btn_inc);

	layout_tile_print(gui_group_get_vptr_elems(grp), -1, -1, (p32) { -1, 0 });

	return grp;
}
//--------------------------------------------------------------------------------------
void gui_spinner_on_input(GUI_ELEM* elm)
{
	gui_group_on_input(elm);

	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;

	if(group->elm_released != NULL)
	{
		if(group->elm_released == spinner->btn_dec && gui_element_to_p32(spinner->btn_dec, gui_input_pos()))
			gui_spinner_add_val(elm, -1);

		if(group->elm_released == spinner->btn_inc && gui_element_to_p32(spinner->btn_inc, gui_input_pos()))
			gui_spinner_add_val(elm, +1);
	}
	else if(group->elm_pressed != NULL && gui_input_pressed_frames() > 20)
	{
		if(group->elm_pressed == spinner->btn_dec && gui_element_to_p32(spinner->btn_dec, gui_input_pos()))
			gui_spinner_add_val(elm, -1);

		if(group->elm_pressed == spinner->btn_inc && gui_element_to_p32(spinner->btn_inc, gui_input_pos()))
			gui_spinner_add_val(elm, +1);
	}
}
//--------------------------------------------------------------------------------------
void gui_spinner_set_txt(GUI_ELEM* elm, const char* txt)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	gui_progressbar_set_txt(spinner->progbar, txt);
}

void gui_spinner_set_val(GUI_ELEM* elm, int32 val)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	gui_progressbar_set_val(spinner->progbar, val);
}

void gui_spinner_add_val(GUI_ELEM* elm, int32 val)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	gui_progressbar_add_val(spinner->progbar, val);
}

int32 gui_spinner_get_val(GUI_ELEM* elm)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	return gui_progressbar_get_val(spinner->progbar);
}

int32 gui_spinner_get_val_min(GUI_ELEM* elm)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	return gui_progressbar_get_val_min(spinner->progbar);
}

int32 gui_spinner_get_val_max(GUI_ELEM* elm)
{
	GUI_GROUP* group = elm->data;
	GUI_SPINNER* spinner = group->data;
	return gui_progressbar_get_val_max(spinner->progbar);
}
//--------------------------------------------------------------------------------------
