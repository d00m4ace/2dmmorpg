typedef struct GUI_DISPLAY
{
	int32 display_id;

	uint32 flags;

	p32 pos;

	VPTR vptr_planes;

	void (*calc_size)(GUI_DISPLAY* dsp);
	void (*calc_pos)(GUI_DISPLAY* dsp);
	void (*show)(GUI_DISPLAY* dsp);
	void (*on_input)(GUI_DISPLAY* dsp);
	void (*on_update)(GUI_DISPLAY* dsp);
	void (*on_free)(GUI_DISPLAY* dsp);

	void* data;
} GUI_DISPLAY;

GUI_DISPLAY* gui_display(int32 display_id);

void gui_display_add_plane(GUI_DISPLAY* dsp, GUI_PLANE* pln);
void gui_display_remove_plane(GUI_DISPLAY* dsp, GUI_PLANE* pln);
GUI_PLANE* gui_display_get_plane(GUI_DISPLAY* dsp, int32 indx);
int32 gui_display_planes_count(GUI_DISPLAY* dsp);

void gui_display_calc_size(GUI_DISPLAY* dsp);
void gui_display_calc_pos(GUI_DISPLAY* dsp);
void gui_display_show(GUI_DISPLAY* dsp);
void gui_display_on_input(GUI_DISPLAY* dsp);
void gui_display_on_update(GUI_DISPLAY* dsp);
void gui_display_on_free(GUI_DISPLAY* dsp);
