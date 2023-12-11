void gui_helper_elems_rest_flag(VPTR* vptr_elems, uint32 flag)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->flags &= ~flag;
	}
}

void gui_helper_elems_add_flag(VPTR* vptr_elems, uint32 flag)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->flags |= flag;
	}
}

void gui_helper_elems_set_flags(VPTR* vptr_elems, uint32 flags)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->flags = flags;
	}
}

int32 gui_helper_elems_find_first_set_flags(VPTR* vptr_elems, uint32 flags)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);

		if(elm->flags & flags)
			return i;
	}

	return -1;
}

int32 gui_helper_elems_find_first_set_no_flags(VPTR* vptr_elems, uint32 flags)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);

		if(!(elm->flags & flags))
			return i;
	}

	return -1;
}

void gui_helper_elems_rest_origin(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->origin = (p32){ 0 };
	}
}

void gui_helper_elems_add_origin(VPTR* vptr_elems, p32 offs)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->origin = p32_add(elm->origin, offs);
	}
}

void gui_helper_elems_set_pln(VPTR* vptr_elems, GUI_PLANE* pln)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->pln = pln;
	}
}

void gui_helper_elems_rest_gui_state(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);

		if(!(elm->flags & (GUI_FLG_DISABLED | GUI_FLG_LOCKED)))
		{
			if(elm->flags & GUI_FLG_SELECTED)
				elm->gui_state = GUI_ELEM_STATE_SELECTED;
			else
				elm->gui_state = GUI_ELEM_STATE_NORMAL;
		}
		else
		{
			elm->gui_state = GUI_ELEM_STATE_NORMAL;

			if(elm->flags & GUI_FLG_DISABLED)
				elm->gui_state = GUI_ELEM_STATE_DISABLED;
		}
	}
}

void gui_helper_elems_rest_pln(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->pln = NULL;
	}
}

void gui_helper_elems_rest_pos(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->pos = (p32){ 0 };
	}
}

void gui_helper_elems_set_size(VPTR* vptr_elems, p32 size)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->size = size;
	}
}


void gui_helper_elems_set_calc_size(VPTR* vptr_elems, void (*calc_size)(GUI_ELEM* elm))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->calc_size = calc_size;
	}
}

void gui_helper_elems_set_calc_pos(VPTR* vptr_elems, void (*calc_pos)(GUI_ELEM* elm, p32 offs))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->calc_pos = calc_pos;
	}
}

void gui_helper_elems_set_show(VPTR* vptr_elems, void (*show)(GUI_ELEM* elm, p32 offs))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->show = show;
	}
}

void gui_helper_elems_set_on_update(VPTR* vptr_elems, void (*on_update)(GUI_ELEM* elm))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_update = on_update;
	}
}

void gui_helper_elems_set_on_input(VPTR* vptr_elems, void (*on_input)(GUI_ELEM* elm))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_input = on_input;
	}
}

void gui_helper_elems_set_on_free(VPTR* vptr_elems, void (*on_free)(GUI_ELEM* elm))
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_free = on_free;
	}
}


void gui_helper_elems_calc_size(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->calc_size(elm);
	}
}

void gui_helper_elems_calc_pos(VPTR* vptr_elems, p32 offs)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->calc_pos(elm, offs);
	}
}

void gui_helper_elems_show(VPTR* vptr_elems, p32 offs)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->show(elm, offs);
	}
}

void gui_helper_elems_on_update(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_update(elm);
	}
}

void gui_helper_elems_on_input(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_input(elm);
	}
}

void gui_helper_elems_on_free(VPTR* vptr_elems)
{
	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		elm->on_free(elm);
	}
}

aabb32 gui_helper_elems_calc_max_aabb(VPTR* vptr_elems)
{
	aabb32 max_aabb = aabb32_init();

	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);
		max_aabb = aabb32_add(max_aabb, aabb32_from_ps(elm->pos, elm->size));
	}

	return max_aabb;
}

aabb32 gui_helper_clip_aabb32(aabb32 view, aabb32 target)
{
	p32 view_size = aabb32_size(view);
	p32 target_size = aabb32_size(target);

	if(target_size.x > view_size.x)
		target_size.x = view_size.x;

	if(target_size.y > view_size.y)
		target_size.y = view_size.y;

	p32 view_origin = aabb32_pos(view);
	p32 target_origin = aabb32_pos(target);

	if(target_origin.x + target_size.x > view_origin.x + view_size.x)
		target_origin.x -= (target_origin.x + target_size.x) - (view_origin.x + view_size.x);

	if(target_origin.y + target_size.y > view_origin.y + view_size.y)
		target_origin.y -= (target_origin.y + target_size.y) - (view_origin.y + view_size.y);

	if(target_origin.x < view_origin.x)
		target_origin.x = view_origin.x;

	if(target_origin.y < view_origin.y)
		target_origin.y = view_origin.y;

	return aabb32_from_ps(target_origin, target_size);
}

p32 gui_helper_txt_calc_content_size(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state)
{
	p32 sz = { 0 };

	if(strlen(txt))
	{
		RD2FONT* fnt = game_font_get(style->font_id[gui_state]);
		sz = rd2_font_print(fnt, txt, 0, 0, 1, 0, 0, FNT_FLG_NONE);
	}

	return sz;
}

p32 gui_helper_txt_calc_content_size_word_wrap(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state, p32 max_size)
{
	p32 sz = { 0 };

	if(strlen(txt))
	{
		RD2IMG scr_save = *rd2_scr_get();

		rd2_scr_get()->width = max_size.x;
		rd2_scr_get()->height = max_size.y;

		RD2FONT* fnt = game_font_get(style->font_id[gui_state]);
		sz = rd2_font_print(fnt, txt, 0, 0, 1, 0, 0, FNT_FLG_WORD_WRAP);

		*rd2_scr_get() = scr_save;
	}

	return sz;
}

p32 gui_helper_str_calc_content_size(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state)
{
	p32 sz = { 0 };

	RD2FONT* fnt = game_font_get(style->font_id[gui_state]);

	if(strlen(txt))
		sz.x = rd2_font_draw_str_calc_width(fnt, txt, 0, 0, 1);

	sz.y = fnt->char_height + fnt->padding_height;

	return sz;
}
