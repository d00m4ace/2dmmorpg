//--------------------------------------------------------------------------------------
GUI_ELEM* gui_group(void)
{
	GUI_ELEM* elm = gui_element();
	GUI_GROUP* group = CALLOC(1, sizeof(GUI_GROUP));

	elm->data = group;

	elm->elem_type = GUI_ELEM_GROUP;
	elm->elem_id = -1;

	elm->style = GUI_STYLE_EMPTY;

	elm->calc_size = gui_group_calc_size;
	elm->calc_pos = gui_group_calc_pos;
	elm->show = gui_group_show;
	elm->on_input = gui_group_on_input;
	elm->on_update = gui_group_on_update;
	elm->on_free = gui_group_on_free;

	group->vptr_elems = vptr_init(16);

	elm->flags = 0;

	return elm;
}
//--------------------------------------------------------------------------------------
void gui_group_add_elm(GUI_ELEM* grp, GUI_ELEM* elm)
{
	GUI_GROUP* group = grp->data;

	if(elm->elem_id == -1)
		elm->elem_id = group->vptr_elems.count;

	vptr_elm_add(&group->vptr_elems, elm);

	elm->pln = NULL;
}
void gui_group_remove_elm(GUI_ELEM* grp, GUI_ELEM* elm)
{
	GUI_GROUP* group = grp->data;

	int32 indx = vptr_elm_indx(&group->vptr_elems, elm);

	if(indx != -1)
		vptr_elm_delete(&group->vptr_elems, indx, 1);

	elm->pln = NULL;
}
GUI_ELEM* gui_group_get_elm(GUI_ELEM* grp, int32 indx)
{
	GUI_GROUP* group = grp->data;

	return vptr_elm(&group->vptr_elems, indx);
}
int32 gui_group_elems_count(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	return group->vptr_elems.count;
}
//--------------------------------------------------------------------------------------
VPTR* gui_group_get_vptr_elems(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;
	return &group->vptr_elems;
}
//--------------------------------------------------------------------------------------
void gui_group_save_inputs(GUI_ELEM* grp, GUI_ELEM** save_inputs)
{
	GUI_GROUP* group = grp->data;

	save_inputs[0] = gui_input_elm_pressed();
	save_inputs[1] = gui_input_elm_released();
	save_inputs[2] = gui_input_elm_focused();

	if(!(grp == gui_input_elm_pressed() || grp == gui_input_elm_released()))
	{
		group->elm_pressed = NULL;
		group->elm_released = NULL;
		group->elm_focused = NULL;
	}

	gui_input_set_elm_pressed(group->elm_pressed);
	gui_input_set_elm_released(group->elm_released);
	gui_input_set_elm_focused(group->elm_focused);
}

void gui_group_restore_inputs(GUI_ELEM* grp, GUI_ELEM** save_inputs)
{
	GUI_GROUP* group = grp->data;

	group->elm_pressed = gui_input_elm_pressed();
	group->elm_released = gui_input_elm_released();
	group->elm_focused = gui_input_elm_focused();

	gui_input_set_elm_pressed(save_inputs[0]);
	gui_input_set_elm_released(save_inputs[1]);
	gui_input_set_elm_focused(save_inputs[2]);
}
//--------------------------------------------------------------------------------------
void gui_group_calc_size(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	aabb32 max_aabb = aabb32_init();

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);

		if(!(elm->flags & GUI_FLG_HIDDEN))
		{
			elm->calc_size(elm);
			elm->calc_pos(elm, (p32) { 0, 0 });
			max_aabb = aabb32_add(max_aabb, aabb32_from_ps(elm->pos, elm->size));
		}
	}

	grp->size = aabb32_size(max_aabb);

	gui_element_calc_size(grp);
}

void gui_group_calc_pos(GUI_ELEM* grp, p32 offs)
{
	GUI_GROUP* group = grp->data;

	gui_element_calc_pos(grp, offs);

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);

		if(!(elm->flags & GUI_FLG_HIDDEN))
			elm->calc_pos(elm, grp->pos);
	}
}

void gui_group_show(GUI_ELEM* grp, p32 offs)
{
	GUI_GROUP* group = grp->data;

	gui_element_show(grp, offs);

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);

		if(!(elm->flags & GUI_FLG_HIDDEN))
			elm->show(elm, offs);
	}
}

void gui_group_on_input(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	gui_element_on_input(grp);

	GUI_ELEM* save_inputs[3];
	gui_group_save_inputs(grp, save_inputs);

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);

		if(!(elm->flags & GUI_FLG_HIDDEN))
			elm->on_input(elm);
	}

	gui_group_restore_inputs(grp, save_inputs);
}

void gui_group_on_update(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	gui_element_on_update(grp);

	GUI_ELEM* save_inputs[3];
	gui_group_save_inputs(grp, save_inputs);

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);

		if(!(elm->flags & GUI_FLG_HIDDEN))
			elm->on_update(elm);
	}

	gui_group_restore_inputs(grp, save_inputs);
}

void gui_group_on_free(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	for(int32 i = 0; i < group->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_group_get_elm(grp, i);
		elm->on_free(elm);
	}

	gui_element_on_free(grp);
}

void gui_group_tile(GUI_ELEM* grp)
{
	GUI_GROUP* group = grp->data;

	layout_tile_print(&group->vptr_elems, -1, -1, (p32) { 1, 1 });
}

//--------------------------------------------------------------------------------------
GUI_ELEM* __gui_group_combo__(GUI_ELEM* elms, ...)
{
	GUI_ELEM* grp = gui_group();

	va_list args;
	va_start(args, elms);

	for(;;)
	{
		GUI_ELEM* elm = va_arg(args, GUI_ELEM*);

		if(elm == NULL)
			break;

		gui_group_add_elm(grp, elm);
	}

	va_end(args);

	gui_group_tile(grp);

	return grp;
}