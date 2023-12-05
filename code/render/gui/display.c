//--------------------------------------------------------------------------------------
GUI_DISPLAY* gui_display(int32 display_id)
{
	GUI_DISPLAY* dsp = CALLOC(1, sizeof(GUI_DISPLAY));

	dsp->calc_size = gui_display_calc_size;
	dsp->calc_pos = gui_display_calc_pos;
	dsp->show = gui_display_show;
	dsp->on_input = gui_display_on_input;
	dsp->on_update = gui_display_on_update;
	dsp->on_free = gui_display_on_free;

	dsp->vptr_planes = vptr_init(16);

	return dsp;
}
//--------------------------------------------------------------------------------------
void gui_display_add_plane(GUI_DISPLAY* dsp, GUI_PLANE* pln)
{
	if(pln->plane_id == -1)
		pln->plane_id = dsp->vptr_planes.count;

	vptr_elm_add(&dsp->vptr_planes, pln);
	pln->dsp = dsp;
}
void gui_display_remove_plane(GUI_DISPLAY* dsp, GUI_PLANE* pln)
{
	int32 indx = vptr_elm_indx(&dsp->vptr_planes, pln);

	if(indx != -1)
		vptr_elm_delete(&dsp->vptr_planes, indx, 1);

	pln->dsp = NULL;
}
GUI_PLANE* gui_display_get_plane(GUI_DISPLAY* dsp, int32 indx)
{
	GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, indx);

	return pln;
}

int32 gui_display_planes_count(GUI_DISPLAY* dsp)
{
	return dsp->vptr_planes.count;
}
//--------------------------------------------------------------------------------------
void gui_display_calc_size(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, i);
		pln->calc_size(pln);
	}
}
void gui_display_calc_pos(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, i);
		pln->calc_pos(pln);
	}
}
void gui_display_show(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, i);
		pln->show(pln);
	}
}
void gui_display_on_update(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, i);
		pln->on_update(pln);
	}
}
void gui_display_on_input(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, dsp->vptr_planes.count - 1 - i);
		pln->on_input(pln);

		if(gui_input_is_processed())
			break;
	}
}
void gui_display_on_free(GUI_DISPLAY* dsp)
{
	for(int32 i = 0; i < dsp->vptr_planes.count; i++)
	{
		GUI_PLANE* pln = vptr_elm(&dsp->vptr_planes, i);
		pln->on_free(pln);
	}
}
//--------------------------------------------------------------------------------------
