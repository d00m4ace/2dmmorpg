//--------------------------------------------------------------------------------------
static GUI_DISPLAY* gui_list_dsp = NULL;
//--------------------------------------------------------------------------------------
void gui_list_display_on_input(GUI_DISPLAY* dsp)
{
	gui_display_on_input(dsp);

	bool is_closed = false;

	if(gui_input_elm_released() != NULL)
	{
		GUI_ELEM* elm_released = gui_input_elm_released();

		if(elm_released->pln == gui_display_get_plane(dsp, 0))
		{
			GUI_ELEM* lst = dsp->data;
			gui_list_set_selected_elm(lst, elm_released);
			is_closed = true;
		}
	}

	GUI_PLANE* pln = gui_display_get_plane(dsp, 0);

	if((gui_input_is_pressed() || gui_input_is_released()) &&
		!aabb32_to_p32(gui_plane_get_aabb32(pln), gui_input_pos_start()))
		is_closed = true;

	if(is_closed)
	{
		GUI_ELEM* lst = dsp->data;
		GUI_LIST* list = lst->data;

		lst->gui_state = GUI_ELEM_STATE_RELEASED;
		lst->gui_state_fade_frames = GUI_STATE_FADE_FRAMES;

		gui_helper_elems_rest_pln(&list->vptr_elems);
		gui_helper_elems_rest_origin(&list->vptr_elems);
		gui_helper_elems_rest_flag(&list->vptr_elems, GUI_FLG_SELECTED);

		gui_remove_display(dsp);
	}
}
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_list(void)
{
	GUI_ELEM* elm = gui_element();
	GUI_LIST* list = CALLOC(1, sizeof(GUI_LIST));

	elm->data = list;

	elm->elem_type = GUI_ELEM_LIST;
	elm->elem_id = -1;

	elm->style = GUI_STYLE_EMPTY;

	elm->calc_size = gui_list_calc_size;
	elm->calc_pos = gui_list_calc_pos;
	elm->show = gui_list_show;
	elm->on_input = gui_list_on_input;
	elm->on_update = gui_list_on_update;
	elm->on_free = gui_list_on_free;

	list->vptr_elems = vptr_init(16);
	list->selected = 0;

	list->flags = GUI_LIST_FLG_VERTICAL;

	elm->flags = 0;

	if(gui_list_dsp == NULL)
	{
		gui_list_dsp = gui_display(-1);

		GUI_PLANE* gui_list_plane = gui_plane(-1, (p32) { game_scr_width(), game_scr_height() });

		gui_display_add_plane(gui_list_dsp, gui_list_plane);

		gui_list_plane->flags |= GUI_FLG_INPUTLOCK | GUI_FLG_SHADOWSCREEN;

		gui_list_dsp->on_input = gui_list_display_on_input;
	}

	return elm;
}

void gui_list_set_vertical(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	list->flags = GUI_LIST_FLG_VERTICAL;
}

void gui_list_set_horizontal(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	list->flags = GUI_LIST_FLG_HORIZONTAL;
}

//--------------------------------------------------------------------------------------
void gui_list_add_elm(GUI_ELEM* lst, GUI_ELEM* elm)
{
	GUI_LIST* list = lst->data;

	if(elm->elem_id == -1)
		elm->elem_id = list->vptr_elems.count;

	vptr_elm_add(&list->vptr_elems, elm);

	elm->pln = NULL;
}
void gui_list_remove_elm(GUI_ELEM* lst, GUI_ELEM* elm)
{
	GUI_LIST* list = lst->data;

	int32 indx = vptr_elm_indx(&list->vptr_elems, elm);

	if(indx != -1)
		vptr_elm_delete(&list->vptr_elems, indx, 1);

	elm->pln = NULL;
}
GUI_ELEM* gui_list_get_elm(GUI_ELEM* lst, int32 indx)
{
	GUI_LIST* list = lst->data;

	return vptr_elm(&list->vptr_elems, indx);
}
int32 gui_list_elems_count(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	return list->vptr_elems.count;
}

int32 gui_list_get_selected(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	return list->selected;
}

void gui_list_set_selected(GUI_ELEM* lst, int32 selected)
{
	GUI_LIST* list = lst->data;

	list->selected = selected;
}

GUI_ELEM* gui_list_get_selected_elm(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	return gui_list_get_elm(lst, list->selected);
}

int32 gui_list_set_selected_elm(GUI_ELEM* lst, GUI_ELEM* elm)
{
	GUI_LIST* list = lst->data;

	list->selected = vptr_elm_indx(&list->vptr_elems, elm);

	ASSERT(list->selected != -1);

	return list->selected;
}
//--------------------------------------------------------------------------------------
void gui_list_calc_size(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	gui_helper_elems_rest_pos(&list->vptr_elems);
	gui_helper_elems_calc_size(&list->vptr_elems);
	aabb32 max_aabb = gui_helper_elems_calc_max_aabb(&list->vptr_elems);
	lst->size = aabb32_size(max_aabb);

	gui_element_calc_size(lst);
}

void gui_list_calc_pos(GUI_ELEM* lst, p32 offs)
{
	GUI_LIST* list = lst->data;

	gui_element_calc_pos(lst, offs);

	{
		GUI_ELEM* elm = gui_list_get_elm(lst, list->selected);
		elm->calc_pos(elm, lst->pos);
	}
}

void gui_list_show(GUI_ELEM* lst, p32 offs)
{
	if(gui_top_display()->data == lst)
		return;

	GUI_LIST* list = lst->data;

	gui_element_show(lst, offs);

	{
		GUI_ELEM* elm = gui_list_get_elm(lst, list->selected);
		elm->show(elm, offs);
	}
}

void gui_list_on_input(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;
	VPTR* vptr_elems = &list->vptr_elems;	

	gui_element_on_input(lst);

	//if(lst == gui_input_elm_pressed())
	if(lst == gui_input_elm_released())
	{
		GUI_PLANE* pln = gui_display_get_plane(gui_list_dsp, 0);

		//!!!TEST
		//vptr_clear(&pln->vptr_elems);
		//vptr_insert(&pln->vptr_elems, &list->vptr_elems, 0, 0, list->vptr_elems.count);
		//!!!OR!!!
		vptr_copy(&pln->vptr_elems, vptr_elems);
		//TEST!!!

		gui_helper_elems_set_pln(vptr_elems, pln);

		if(list->flags & GUI_LIST_FLG_VERTICAL)
			layout_tile_print(vptr_elems, -1, 1, (p32) { 1, 1 });
		else
			layout_tile_print(vptr_elems, -1, -1, (p32) { 1, 1 });

		p32 selected_pos;

		gui_helper_elems_rest_gui_state(vptr_elems);
		gui_helper_elems_rest_flag(vptr_elems, GUI_FLG_SELECTED);

		{
			GUI_ELEM* elm = gui_list_get_selected_elm(lst);
			elm->flags |= GUI_FLG_SELECTED;
			selected_pos = elm->origin;
		}

		gui_helper_elems_calc_pos(vptr_elems, (p32) { 0 });
		aabb32 max_aabb = gui_helper_elems_calc_max_aabb(vptr_elems);

		pln->size = aabb32_size(max_aabb);
		pln->origin = lst->pos;

		aabb32 target = gui_helper_clip_aabb32(aabb32_from_p((p32) { game_scr_width(), game_scr_height() }), aabb32_from_ps(pln->origin, pln->size));
		pln->origin = aabb32_pos(target);
		pln->size = aabb32_size(target);

		pln->pos = (p32){ 0 };
		pln->scrolling.scroll_pos = (d2v){ 0 };
		gui_plane_scroll_to(pln, selected_pos);

		gui_list_dsp->data = lst;
		gui_add_display(gui_list_dsp);
	}
}

void gui_list_on_update(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	gui_element_on_update(lst);

	{
		GUI_ELEM* elm = gui_list_get_elm(lst, list->selected);
		elm->gui_state = lst->gui_state;
		//elm->on_update(elm);
	}
}

void gui_list_on_free(GUI_ELEM* lst)
{
	GUI_LIST* list = lst->data;

	for(int32 i = 0; i < list->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = gui_list_get_elm(lst, i);
		elm->on_free(elm);
	}

	gui_element_on_free(lst);
}
//--------------------------------------------------------------------------------------
