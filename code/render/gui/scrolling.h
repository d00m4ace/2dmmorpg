//--------------------------------------------------------------------------------------
#define GUI_SCROLLING_FLG_NONE (0U)
#define GUI_SCROLLING_FLG_V_ENABLE (1U<<0)
#define GUI_SCROLLING_FLG_H_ENABLE (1U<<1)
#define GUI_SCROLLING_FLG_WHEEL_ENABLE (1U<<2)
#define GUI_SCROLLING_FLG_NO_LIMITS (1U<<3)
//--------------------------------------------------------------------------------------
typedef struct GUI_SCROLLING
{
	d2v scroll_pos;
	d2v scroll_to;
	d2v scroll_v;

	int32 show_scrollbar_frames;

	int32 drag_delta_scale;
	int32 mouse_wheel_scale;

	uint32 flags;
} GUI_SCROLLING;

GUI_SCROLLING gui_scrolling(int32 drag_delta_scale, int32 mouse_wheel_scale);

void gui_scrolling_calc_pos(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size);
void gui_scrolling_show(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size);
void gui_scrolling_on_input(GUI_SCROLLING* scr, p32 pos, p32 max_size, p32 size);
