//--------------------------------------------------------------------------------------
#define GUI_FLG_UNSCROLLABLE (1U<<29)
#define GUI_FLG_INPUTLOCK (1U<<30)
#define GUI_FLG_SHADOWSCREEN (1U<<31)
//--------------------------------------------------------------------------------------
typedef struct GUI_PLANE
{
	int32 plane_id;

	uint32 flags;

	p32 size;
	p32 pos;

	p32 origin;

	aabb32 max_aabb;

	GUI_SCROLLING scrolling;

	GUI_STYLE_ENUM style;

	GUI_ELEM_STATE_ENUM gui_state;

	VPTR vptr_elems;

	GUI_DISPLAY* dsp;

	void (*calc_size)(GUI_PLANE* pln);
	void (*calc_pos)(GUI_PLANE* pln);
	void (*show)(GUI_PLANE* pln);
	void (*on_input)(GUI_PLANE* pln);
	void (*on_update)(GUI_PLANE* pln);
	void (*on_free)(GUI_PLANE* pln);

	void* data;
} GUI_PLANE;

GUI_PLANE* gui_plane(int32 plane_id, p32 size);

void gui_plane_add_elm(GUI_PLANE* pln, GUI_ELEM* elm);
void gui_plane_remove_elm(GUI_PLANE* pln, GUI_ELEM* elm);
GUI_ELEM* gui_plane_get_elm(GUI_PLANE* pln, int32 indx);
int32 gui_plane_elems_count(GUI_PLANE* pln);

aabb32 gui_plane_get_aabb32(GUI_PLANE* pln);

// to in screen coords
void gui_plane_scroll_to(GUI_PLANE* pln, p32 to);

void gui_plane_calc_size(GUI_PLANE* pln);
void gui_plane_calc_pos(GUI_PLANE* pln);
void gui_plane_show(GUI_PLANE* pln);
void gui_plane_on_input(GUI_PLANE* pln);
void gui_plane_on_update(GUI_PLANE* pln);
void gui_plane_on_free(GUI_PLANE* pln);
