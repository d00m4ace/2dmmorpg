//--------------------------------------------------------------------------------------
int32 gui_style_calc_valign(VERTICAL_ALIGN_ENUM algn, int32 dst_h, int32 src_h)
{
	if(algn == VALIGN_TOP || algn == VALIGN_NONE)
		return 0;
	else if(algn == VALIGN_MIDDLE)
		return (dst_h - src_h) >> 1;
	else //if(algn == VALIGN_BOTTOM)
		return (dst_h - src_h);
}

int32 gui_style_calc_halign(HORIZONTAL_ALIGN_ENUM algn, int32 dst_w, int32 src_w)
{
	if(algn == HALIGN_LEFT || algn == HALIGN_NONE)
		return 0;
	else if(algn == HALIGN_CENTER)
		return (dst_w - src_w) >> 1;
	else //if(algn == HALIGN_RIGHT)
		return (dst_w - src_w);
}

void gui_style_calc_size(GUI_ELEM* elm)
{
	GUI_STYLE* style = gui_style_get(elm->style);

	if(style->size_min.x != -1)
		elm->size.x = MAX(elm->size.x, style->size_min.x);
	if(style->size_min.y != -1)
		elm->size.y = MAX(elm->size.y, style->size_min.y);

	if(style->size_max.x != -1)
		elm->size.x = MIN(elm->size.x, style->size_max.x);
	if(style->size_max.y != -1)
		elm->size.y = MIN(elm->size.y, style->size_max.y);
}

p32 gui_style_size_min(GUI_STYLE_ENUM style_id)
{
	return gui_style_get(style_id)->size_min;
}

p32 gui_style_size_max(GUI_STYLE_ENUM style_id)
{
	return gui_style_get(style_id)->size_max;
}
//--------------------------------------------------------------------------------------
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_ZX_RUS_THIN_BORDER
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_ZX_RUS_THIN
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_ZX_RUS
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_BOLD_RUS
#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_DOS_RUS
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_ZX_RUS_BORDER
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_BOLD_RUS_BORDER
//#define GUI_DEFAULT_GAME_FONT_ID GAME_FONT_ID_DOS_RUS_BORDER

#define GUI_DEFAULT_BUTTON_HEIGHT 24

#define GUI_DEFAULT_BUTTON_WIDTH_MIN 48
#define GUI_DEFAULT_BUTTON_WIDTH_MAX 180

#define GUI_DEFAULT_BUTTON_SIZE_MIN (p32){ GUI_DEFAULT_BUTTON_WIDTH_MIN,GUI_DEFAULT_BUTTON_HEIGHT }
#define GUI_DEFAULT_BUTTON_SIZE_MAX (p32){ GUI_DEFAULT_BUTTON_WIDTH_MAX,GUI_DEFAULT_BUTTON_HEIGHT }

#define GUI_DEFAULT_COLOR PAL_WHITE

#define GUI_DEFAULT_BACKCOLOR PAL_ROYALBLUE
#define GUI_DEFAULT_BORDERCOLOR PAL_STEELBLUE

#define GUI_DEFAULT_BACKCOLOR_LIGHT PAL_SKYBLUE
#define GUI_DEFAULT_BORDERCOLOR_LIGHT PAL_SKYBLUE

#define GUI_DEFAULT_TRANSPARENT_COLOR PAL_TRANSPARENT
//--------------------------------------------------------------------------------------
GUI_STYLE gui_style_button(void)
{
	GUI_STYLE style;

	style.font_id[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_GAME_FONT_ID;

	style.color[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_FOCUSED] = PAL_BLACK;
	style.color[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_DISABLED] = PAL_DARKGRAY;

	style.color_background[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_BACKCOLOR;
	style.color_background[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_BACKCOLOR_LIGHT;
	style.color_background[GUI_ELEM_STATE_RELEASED] = PAL_WHITE;
	style.color_background[GUI_ELEM_STATE_FOCUSED] = PAL_WHITE;
	style.color_background[GUI_ELEM_STATE_SELECTED] = PAL_SKYBLUE;
	style.color_background[GUI_ELEM_STATE_DISABLED] = PAL_GRAY;

	style.color_border[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_BACKCOLOR_LIGHT;
	style.color_border[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_BORDERCOLOR_LIGHT;
	style.color_border[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_BACKCOLOR;
	style.color_border[GUI_ELEM_STATE_FOCUSED] = PAL_GREEN;
	style.color_border[GUI_ELEM_STATE_SELECTED] = PAL_GOLD;
	style.color_border[GUI_ELEM_STATE_DISABLED] = PAL_DARKGRAY;

	style.size_min = GUI_DEFAULT_BUTTON_SIZE_MIN;
	style.size_max = GUI_DEFAULT_BUTTON_SIZE_MAX;

	style.background = GUI_BACKGROUND_BUBBLE;

	return style;
}
//--------------------------------------------------------------------------------------
GUI_STYLE gui_style_button_tile(void)
{
	GUI_STYLE style = gui_style_button();

	style.size_min = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MIN + 1,GUI_DEFAULT_BUTTON_WIDTH_MIN + 1 };
	style.size_max = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MAX + 1,GUI_DEFAULT_BUTTON_WIDTH_MAX + 1 };

	return style;
}

GUI_STYLE gui_style_button_long(void)
{
	GUI_STYLE style = gui_style_button();

	style.size_min = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MIN * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };
	style.size_max = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MAX * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };

	return style;
}

GUI_STYLE gui_style_button_keyboard(void)
{
	GUI_STYLE style = gui_style_button();

	style.size_min = (p32) { 31,30 };
	style.size_max = (p32) { 31,30 };

	//style.background = GUI_BACKGROUND_BOX;
	//style.background = GUI_BACKGROUND_TILE;
	style.background = GUI_BACKGROUND_BUBBLE;

	return style;
}

GUI_STYLE gui_style_text(void)
{
	GUI_STYLE style = gui_style_button();

	style.color_background[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_TRANSPARENT_COLOR;

	style.color_border[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_TRANSPARENT_COLOR;

	//style.size_min = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MIN * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };
	//style.size_max = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MAX * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };

	//style.size_min = (p32) { -1,-1 };
	//style.size_max = (p32) { -1,-1 };

	style.size_min = (p32) { -1,GUI_DEFAULT_BUTTON_HEIGHT };
	style.size_max = (p32) { -1,GUI_DEFAULT_BUTTON_HEIGHT };

	return style;
}

GUI_STYLE gui_style_text_border(void)
{
	GUI_STYLE style = gui_style_text();

	style.font_id[GUI_ELEM_STATE_NORMAL] = GAME_FONT_ID_DOS_RUS_BORDER;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GAME_FONT_ID_DOS_RUS_BORDER;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GAME_FONT_ID_DOS_RUS_BORDER;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GAME_FONT_ID_DOS_RUS_BORDER;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GAME_FONT_ID_DOS_RUS_BORDER;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GAME_FONT_ID_DOS_RUS_BORDER;

	return style;
}

GUI_STYLE gui_style_text_small(void)
{
	GUI_STYLE style = gui_style_text();

	style.font_id[GUI_ELEM_STATE_NORMAL] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GAME_FONT_ID_ZX_RUS_THIN;

	return style;
}

GUI_STYLE gui_style_text_small_border(void)
{
	GUI_STYLE style = gui_style_text();

	style.font_id[GUI_ELEM_STATE_NORMAL] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GAME_FONT_ID_ZX_RUS_THIN_BORDER;

	return style;
}

GUI_STYLE gui_style_text_input(void)
{
	GUI_STYLE style = gui_style_button();

	style.color_background[GUI_ELEM_STATE_NORMAL] = PAL_SILVER;
	style.color_background[GUI_ELEM_STATE_PRESSED] = PAL_SILVER;
	style.color_background[GUI_ELEM_STATE_RELEASED] = PAL_SILVER;
	style.color_background[GUI_ELEM_STATE_FOCUSED] = PAL_SILVER;
	style.color_background[GUI_ELEM_STATE_SELECTED] = PAL_SILVER;
	style.color_background[GUI_ELEM_STATE_DISABLED] = PAL_GRAY;

	style.color_border[GUI_ELEM_STATE_PRESSED] = PAL_GREEN;
	style.color_border[GUI_ELEM_STATE_FOCUSED] = PAL_GREEN;

	style.size_min = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MIN * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };
	style.size_max = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MAX * 2 + 1,GUI_DEFAULT_BUTTON_HEIGHT };

	return style;
}

GUI_STYLE gui_style_text_input_long(void)
{
	GUI_STYLE style = gui_style_text_input();

	style.size_min = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MIN * 4 + 1,GUI_DEFAULT_BUTTON_HEIGHT };
	style.size_max = (p32) { GUI_DEFAULT_BUTTON_WIDTH_MAX * 4 + 1,GUI_DEFAULT_BUTTON_HEIGHT };

	return style;
}

//GUI_STYLE_TEXT_INPUT

GUI_STYLE gui_style_progressbar(void)
{
	GUI_STYLE style = gui_style_button();

	style.size_min = (p32) { 100 + 2,16 };
	style.size_max = (p32) { 100 + 2,16 };

	style.color_background[GUI_ELEM_STATE_PRESSED] = style.color_background[GUI_ELEM_STATE_NORMAL];
	style.color_border[GUI_ELEM_STATE_PRESSED] = style.color_border[GUI_ELEM_STATE_NORMAL];
	style.color[GUI_ELEM_STATE_DISABLED] = PAL_SILVER;

	//style.background = GUI_BACKGROUND_BOX;
	style.background = GUI_BACKGROUND_TILE;
	//style.background = GUI_BACKGROUND_BUBBLE;

	return style;
}

GUI_STYLE gui_style_spinner(void)
{
	GUI_STYLE style = gui_style_progressbar();

	style.size_min = (p32) { 100 - 16 * 2,16 };
	style.size_max = (p32) { 100 - 16 * 2,16 };

	style.background = GUI_BACKGROUND_TILE;

	return style;
}

GUI_STYLE gui_style_spinner_button(void)
{
	GUI_STYLE style = gui_style_button();

	style.size_min = (p32) { 16,16 };
	style.size_max = (p32) { 16,16 };

	style.background = GUI_BACKGROUND_TILE;

	return style;
}

//--------------------------------------------------------------------------------------
GUI_STYLE gui_style_plane(void)
{
	GUI_STYLE style = gui_style_button();

	style.color_background[GUI_ELEM_STATE_NORMAL] = PAL_STEELBLUE;
	style.color_background[GUI_ELEM_STATE_PRESSED] = PAL_STEELBLUE;
	style.color_background[GUI_ELEM_STATE_RELEASED] = PAL_STEELBLUE;
	style.color_background[GUI_ELEM_STATE_FOCUSED] = PAL_STEELBLUE;
	style.color_background[GUI_ELEM_STATE_SELECTED] = PAL_STEELBLUE;
	style.color_background[GUI_ELEM_STATE_DISABLED] = PAL_STEELBLUE;

	style.color_border[GUI_ELEM_STATE_NORMAL] = PAL_STEELBLUE;
	style.color_border[GUI_ELEM_STATE_PRESSED] = PAL_STEELBLUE;
	style.color_border[GUI_ELEM_STATE_RELEASED] = PAL_STEELBLUE;
	style.color_border[GUI_ELEM_STATE_FOCUSED] = PAL_STEELBLUE;
	style.color_border[GUI_ELEM_STATE_SELECTED] = PAL_STEELBLUE;
	style.color_border[GUI_ELEM_STATE_DISABLED] = PAL_STEELBLUE;

	style.size_min = (p32) { -1,-1 };
	style.size_max = (p32) { -1,-1 };

	style.background = GUI_BACKGROUND_BOX;

	return style;
}
//--------------------------------------------------------------------------------------
GUI_STYLE gui_style_empty(void)
{
	GUI_STYLE style;

	style.font_id[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_GAME_FONT_ID;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_GAME_FONT_ID;

	style.size_min = (p32) { -1,-1 };
	style.size_max = (p32) { -1,-1 };

	style.color[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_COLOR;
	style.color[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_COLOR;

	style.color_background[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_background[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_TRANSPARENT_COLOR;

	style.color_border[GUI_ELEM_STATE_NORMAL] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_PRESSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_RELEASED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_FOCUSED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_SELECTED] = GUI_DEFAULT_TRANSPARENT_COLOR;
	style.color_border[GUI_ELEM_STATE_DISABLED] = GUI_DEFAULT_TRANSPARENT_COLOR;

	style.background = GUI_BACKGROUND_NONE;

	return style;
}

GUI_STYLE gui_style_empty_thin(void)
{
	GUI_STYLE style = gui_style_empty();

	style.font_id[GUI_ELEM_STATE_NORMAL] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GAME_FONT_ID_ZX_RUS_THIN;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GAME_FONT_ID_ZX_RUS_THIN;

	return style;
}

GUI_STYLE gui_style_empty_small(void)
{
	GUI_STYLE style = gui_style_empty();

	style.font_id[GUI_ELEM_STATE_NORMAL] = GAME_FONT_ID_DOS_RUS;
	style.font_id[GUI_ELEM_STATE_PRESSED] = GAME_FONT_ID_DOS_RUS;
	style.font_id[GUI_ELEM_STATE_RELEASED] = GAME_FONT_ID_DOS_RUS;
	style.font_id[GUI_ELEM_STATE_FOCUSED] = GAME_FONT_ID_DOS_RUS;
	style.font_id[GUI_ELEM_STATE_SELECTED] = GAME_FONT_ID_DOS_RUS;
	style.font_id[GUI_ELEM_STATE_DISABLED] = GAME_FONT_ID_DOS_RUS;

	return style;
}

//--------------------------------------------------------------------------------------
void gui_style_init(void)
{
	gui_style_set(GUI_STYLE_NONE, gui_style_empty());

	gui_style_set(GUI_STYLE_BUTTON, gui_style_button());
	gui_style_set(GUI_STYLE_BUTTON_TILE, gui_style_button_tile());
	gui_style_set(GUI_STYLE_BUTTON_LONG, gui_style_button_long());
	gui_style_set(GUI_STYLE_BUTTON_KEYBOARD, gui_style_button_keyboard());
	
	gui_style_set(GUI_STYLE_TEXT, gui_style_text());
	gui_style_set(GUI_STYLE_TEXT_BORDER, gui_style_text_border());
	gui_style_set(GUI_STYLE_TEXT_SMALL, gui_style_text_small());
	gui_style_set(GUI_STYLE_TEXT_SMALL_BORDER, gui_style_text_small_border());

	gui_style_set(GUI_STYLE_TEXT_INPUT, gui_style_text_input());
	gui_style_set(GUI_STYLE_TEXT_INPUT_LONG, gui_style_text_input_long());

	gui_style_set(GUI_STYLE_PROGRESSBAR, gui_style_progressbar());
	gui_style_set(GUI_STYLE_SPINNER, gui_style_spinner());
	gui_style_set(GUI_STYLE_SPINNER_BUTTON, gui_style_spinner_button());

	gui_style_set(GUI_STYLE_EMPTY, gui_style_empty());
	gui_style_set(GUI_STYLE_EMPTY_TEXT_THIN, gui_style_empty_thin());
	gui_style_set(GUI_STYLE_EMPTY_TEXT_SMALL, gui_style_empty_small());

	gui_style_set(GUI_STYLE_PLANE, gui_style_plane());
}
//--------------------------------------------------------------------------------------
