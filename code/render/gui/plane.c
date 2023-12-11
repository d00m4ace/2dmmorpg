GUI_PLANE* gui_plane(int32 plane_id, p32 size)
{
	GUI_PLANE* pln = CALLOC(1, sizeof(GUI_PLANE));

	pln->calc_size = gui_plane_calc_size;
	pln->calc_pos = gui_plane_calc_pos;
	pln->show = gui_plane_show;
	pln->on_input = gui_plane_on_input;
	pln->on_update = gui_plane_on_update;
	pln->on_free = gui_plane_on_free;

	pln->vptr_elems = vptr_init(16);

	pln->plane_id = plane_id;
	pln->size = size;

	pln->scrolling = gui_scrolling(3, 8);

	pln->flags = 0;

	pln->style = GUI_STYLE_PLANE;

	pln->gui_state = GUI_ELEM_STATE_NORMAL;

	return pln;
}

void gui_plane_add_elm(GUI_PLANE* pln, GUI_ELEM* elm)
{
	if(elm->elem_id == -1)
		elm->elem_id = pln->vptr_elems.count;

	vptr_elm_add(&pln->vptr_elems, elm);
	elm->pln = pln;
}
void gui_plane_remove_elm(GUI_PLANE* pln, GUI_ELEM* elm)
{
	int32 indx = vptr_elm_indx(&pln->vptr_elems, elm);

	if(indx != -1)
		vptr_elm_delete(&pln->vptr_elems, indx, 1);

	elm->pln = NULL;
}

GUI_ELEM* gui_plane_get_elm(GUI_PLANE* pln, int32 indx)
{
	GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, indx);

	return elm;
}

int32 gui_plane_elems_count(GUI_PLANE* pln)
{
	return pln->vptr_elems.count;
}

aabb32 gui_plane_get_aabb32(GUI_PLANE* pln)
{
	return aabb32_from_ps(pln->pos, pln->size);
}

void gui_plane_scroll_to(GUI_PLANE* pln, p32 to)
{
	pln->scrolling.scroll_to = p32_to_d2v(p32_invers(p32_sub(p32_sub(to, pln->pos), p32_from_d2v(pln->scrolling.scroll_pos))));
}

void gui_plane_calc_size(GUI_PLANE* pln)
{
	for(int32 i = 0; i < pln->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, i);
		elm->calc_size(elm);
	}
}
void gui_plane_calc_pos(GUI_PLANE* pln)
{
	pln->pos = p32_add(pln->dsp->pos, pln->origin);

	pln->max_aabb = aabb32_init();

	p32 offs = p32_add(pln->pos, p32_from_d2v(pln->scrolling.scroll_pos));

	for(int32 i = 0; i < pln->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, i);
		elm->calc_pos(elm, offs);
		pln->max_aabb = aabb32_add(pln->max_aabb, aabb32_from_ps(elm->pos, elm->size));
	}

	gui_scrolling_calc_pos(&pln->scrolling, pln->pos, aabb32_size(pln->max_aabb), pln->size);
}
void gui_plane_show(GUI_PLANE* pln)
{
	GUI_STYLE* style = gui_style_get(pln->style);
	p32 size = pln->size;
	p32 pos = pln->pos;

	aabb32 pnl_aabb = aabb32_from_ps(pos, size);

	//rd2_rect(pos.x, pos.y, size.x, size.y, ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255));

	if(pln->flags & GUI_FLG_SHADOWSCREEN)
		rd2_rect_fill(0, 0, rd2_scr_width(), rd2_scr_height(),
			ARGB32_SET_ALPHA(gui_color(style->color_background[GUI_ELEM_STATE_DISABLED]), 64 * 3));

	rd2_scr_stack_push(size.x, size.y, gui_color(PAL_TRANSPARENT));

	{
		rd2c color_border = gui_color(style->color_border[pln->gui_state]);
		rd2c color_background = gui_color(style->color_background[pln->gui_state]);

		p32 offs = { 0,0 };

		switch(style->background)
		{
			case GUI_BACKGROUND_BOX:
			rd2_box(offs.x, offs.y, size.x, size.y, color_background, color_border);
			break;
			case GUI_BACKGROUND_TILE:
			rd2_tile(offs.x, offs.y, size.x, size.y, color_background, color_border);
			break;
			case GUI_BACKGROUND_BUBBLE:
			rd2_bubble(offs.x, offs.y, size.x, size.y, color_background, color_border);
			break;
		}
	}

	p32 pos_inv = p32_invers(pos);

	for(int32 i = 0; i < pln->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, i);

		if(!aabb32_to_aabb32(pnl_aabb, aabb32_from_ps(elm->pos, elm->size)))
			continue;

		elm->show(elm, pos_inv);
	}

	gui_scrolling_show(&pln->scrolling, (p32) { 0 }, aabb32_size(pln->max_aabb), pln->size);

	rd2_scr_stack_pop_show(pos);
}
void gui_plane_on_update(GUI_PLANE* pln)
{
	for(int32 i = 0; i < pln->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, i);
		elm->on_update(elm);
	}
}

void gui_plane_on_input(GUI_PLANE* pln)
{
	p32 size = pln->size;
	p32 pos = pln->pos;

	aabb32 pnl_aabb = aabb32_from_ps(pos, size);

	if((gui_input_is_pressed() || gui_input_is_released()) &&
		!aabb32_to_p32(pnl_aabb, gui_input_pos_start()))
	{
		if(pln->flags & GUI_FLG_INPUTLOCK)
			gui_input_set_processed(true);
		return;
	}

	if(gui_input_elm_pressed() == NULL || (!(gui_input_elm_pressed()->flags & GUI_FLG_MOUSELOCK)))
	{
		if(!(pln->flags & GUI_FLG_UNSCROLLABLE))
			gui_scrolling_on_input(&pln->scrolling, pln->pos, aabb32_size(pln->max_aabb), pln->size);
	}

	if((gui_input_is_pressed() || gui_input_is_released()) &&
		!aabb32_to_p32(pnl_aabb, gui_input_pos()))
	{
		gui_input_set_processed(true);
		return;
	}

	for(int32 i = 0; i < pln->vptr_elems.count; i++)
	{
		GUI_ELEM* elm = vptr_elm(&pln->vptr_elems, i);

		if(!aabb32_to_aabb32(pnl_aabb, aabb32_from_ps(elm->pos, elm->size)))
			continue;

		elm->on_input(elm);

		if(gui_input_is_processed())
			break;
	}

	gui_input_set_processed(true);
}
void gui_plane_on_free(GUI_PLANE* pln)
{
}

void gui_layout_tile(GUI_PLANE* pln)
{
	layout_tile_print(&pln->vptr_elems, pln->size.x, -1, (p32) { 1, 1 });
}

void gui_plane_add_new_line(GUI_PLANE* pln)
{
	gui_plane_add_elm(pln, gui_space_new_line());
}

void gui_plane_add_space(GUI_PLANE* pln)
{
	gui_plane_add_elm(pln, gui_space());
}