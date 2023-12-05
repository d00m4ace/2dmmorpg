int32 layout_find_first_intersect(VPTR* vptr_elems, int32 istart, int32 iend, GUI_ELEM* elm)
{
	if(istart < 0 || iend < 1)
		return -1;

	aabb32 elm_aabb = aabb32_from_ps(elm->origin, elm->size);

	for(int32 i = iend - 1; i >= istart; i--)
	{
		GUI_ELEM* elm_to = vptr_elm(vptr_elems, i);

		if(elm_to->size.x == 0 || elm_to->size.y == 0)
			continue;

		aabb32 elm_to_aabb = aabb32_from_ps(elm_to->origin, elm_to->size);
		if(aabb32_to_aabb32(elm_aabb, elm_to_aabb))
			return i;
	}

	return -1;
}

void layout_tile_pack(VPTR* vptr_elems, int32 max_width, p32 padding)
{
	p32 pos = { 0,0 };

	gui_helper_elems_calc_size(vptr_elems);

	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);

		elm->origin = pos;
		int32 intersect_indx = layout_find_first_intersect(vptr_elems, 0, i, elm);

		int32 scan_line_count = 0;

		while(intersect_indx != -1)
		{
			GUI_ELEM* elm_to = vptr_elm(vptr_elems, intersect_indx);

			pos.x = elm_to->origin.x + elm_to->size.x + padding.x;

			if((max_width != -1 && pos.x + elm->size.x > max_width) ||
				(pos.x != 0 && elm->elem_type == GUI_ELEM_SPACE_NEW_LINE))
			{
				pos.x = 0;

				if(scan_line_count > 0 || elm->elem_type == GUI_ELEM_SPACE_NEW_LINE)
				{
					pos.y += padding.y;
					scan_line_count = 0;
				}
				else
				{
					scan_line_count++;
				}
			}

			elm->origin = pos;
			intersect_indx = layout_find_first_intersect(vptr_elems, 0, i, elm);
		}

		elm->origin = pos;
	}
}

void layout_tile_print(VPTR* vptr_elems, int32 max_width, int32 max_tiles_per_line, p32 padding)
{
	p32 pos = { 0,0 };

	gui_helper_elems_calc_size(vptr_elems);

	int32 tiles_count = 0;
	int32 max_height = 0;

	for(int32 i = 0; i < vptr_elems->count; i++)
	{
		GUI_ELEM* elm = vptr_elm(vptr_elems, i);

		elm->origin = pos;

		while(1)
		{
			if((max_tiles_per_line != -1 && tiles_count == max_tiles_per_line) ||
				(max_width != -1 && pos.x + elm->size.x > max_width) ||
				(pos.x != 0 && elm->elem_type == GUI_ELEM_SPACE_NEW_LINE))
			{
				pos.x = 0;
				pos.y += max_height + padding.y;
				tiles_count = 0;
				max_height = 0;
				continue;
			}

			break;
		}

		elm->origin = pos;

		if(elm->size.x != 0 && elm->size.y != 0)
			pos.x += elm->size.x + padding.x;

		max_height = MAX(max_height, elm->size.y);

		if(elm->elem_type != GUI_ELEM_SPACE_NEW_LINE)
			tiles_count++;
	}
}

