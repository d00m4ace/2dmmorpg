//--------------------------------------------------------------------------------------
typedef struct GUI_STYLE
{
	// 0 NORMAL | FOCUSED | PRESSED | DISABLED
	int32 font_id[GUI_ELEM_STATE_ENUM_COUNT];

	p32 size_min;
	p32 size_max;

	int8 color[GUI_ELEM_STATE_ENUM_COUNT];
	int8 color_background[GUI_ELEM_STATE_ENUM_COUNT];
	int8 color_border[GUI_ELEM_STATE_ENUM_COUNT];

	GUI_BACKGROUND_ENUM background;
} GUI_STYLE;
//--------------------------------------------------------------------------------------
int32 gui_style_calc_valign(VERTICAL_ALIGN_ENUM algn, int32 dst_h, int32 src_h);
int32 gui_style_calc_halign(HORIZONTAL_ALIGN_ENUM algn, int32 dst_w, int32 src_w);
void gui_style_calc_size(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
p32 gui_style_size_min(GUI_STYLE_ENUM style_id);
p32 gui_style_size_max(GUI_STYLE_ENUM style_id);
//--------------------------------------------------------------------------------------
GUI_STYLE gui_style_button(void);
GUI_STYLE gui_style_button_tile(void);
GUI_STYLE gui_style_button_long(void);
GUI_STYLE gui_style_button_keyboard(void);
GUI_STYLE gui_style_text_input(void);
GUI_STYLE gui_style_text_input_long(void);
GUI_STYLE gui_style_progressbar(void);
GUI_STYLE gui_style_spinner(void);
GUI_STYLE gui_style_spinner_button(void);
GUI_STYLE gui_style_empty(void);
GUI_STYLE gui_style_empty_thin(void);
GUI_STYLE gui_style_empty_small(void);
//--------------------------------------------------------------------------------------
void gui_style_init(void);
//--------------------------------------------------------------------------------------
