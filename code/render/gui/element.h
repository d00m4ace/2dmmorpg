//--------------------------------------------------------------------------------------
#define GUI_FLG_NONE (0U)

#define GUI_FLG_HIDDEN (1U<<0)

#define GUI_FLG_DIRT (1U<<1)

#define GUI_FLG_DISABLED (1U<<2)
#define GUI_FLG_LOCKED (1U<<3)

#define GUI_FLG_MOUSELOCK (1U<<4)

#define GUI_FLG_FOCUSABLE (1U<<5)
#define GUI_FLG_SELECTABLE (1U<<6)

#define GUI_FLG_SELECTED (1U<<7)

#define GUI_FLG_WORD_WRAP (1U<<8)
//--------------------------------------------------------------------------------------
#define GUI_STATE_FADE_FRAMES 10
//--------------------------------------------------------------------------------------
typedef struct GUI_ELEM
{
	GUI_ELEM_ENUM elem_type;
	int32 elem_id;

	uint32 flags;

	p32 size;
	p32 pos;

	p32 origin;

	GUI_STYLE_ENUM style;

	GUI_ELEM_STATE_ENUM gui_state;
	int32 gui_state_fade_frames;

	HORIZONTAL_ALIGN_ENUM content_halign;
	VERTICAL_ALIGN_ENUM content_valign;

	GUI_PLANE* pln;

	void (*calc_size)(GUI_ELEM* elm);
	void (*calc_pos)(GUI_ELEM* elm, p32 offs);
	void (*show)(GUI_ELEM* elm, p32 offs);
	void (*on_input)(GUI_ELEM* elm);
	void (*on_update)(GUI_ELEM* elm);
	void (*on_free)(GUI_ELEM* elm);

	void* data;

	void* user_data;
} GUI_ELEM;
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_element(void);

void gui_element_calc_size(GUI_ELEM* elm);
void gui_element_calc_pos(GUI_ELEM* elm, p32 offs);
void gui_element_show(GUI_ELEM* elm, p32 offs);
void gui_element_on_input(GUI_ELEM* elm);
void gui_element_on_update(GUI_ELEM* elm);
void gui_element_on_free(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
bool gui_element_to_p32(GUI_ELEM* elm, p32 p);
aabb32 gui_element_get_aabb32(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_element_set_style_and_size_min(GUI_ELEM* elm, GUI_STYLE_ENUM style);
void gui_element_set_style_and_size_max(GUI_ELEM* elm, GUI_STYLE_ENUM style);
//--------------------------------------------------------------------------------------
