//--------------------------------------------------------------------------------------
#include "element.c"
#include "group.c"
#include "guistyles.c"
#include "scrolling.c"
#include "plane.c"
#include "display.c"
#include "text.c"
#include "button.c"
#include "space.c"
#include "progressbar.c"
#include "spinner.c"
#include "composite.c"
#include "tabs.c"
#include "toggle.c"
#include "textinput.c"
#include "list.c"
#include "layouts.c"
#include "helper.c"
#include "keyboard.c"
//--------------------------------------------------------------------------------------
typedef struct GUI_INPUT
{
	bool is_pressed;
	bool is_released;
	bool is_drag;

	bool is_processed;

	int32 pressed_frames;

	p32 pos;
	p32 pos_last;

	p32 pos_start;
	p32 drag_delta;

	int32 drag_delta_min;

	GUI_ELEM* elm_pressed;
	GUI_ELEM* elm_released;
	GUI_ELEM* elm_focused;
} GUI_INPUT;
//--------------------------------------------------------------------------------------
static GUI_INPUT gui_inpt;
static VPTR vptr_displays;
static GUI_STYLE gui_styles[GUI_STYLE_ENUM_COUNT];
static uint32 gui_palette[PALETTE_COLORS_ENUM_COUNT];
//--------------------------------------------------------------------------------------
void gui_init(void)
{
	vptr_displays = vptr_init(16);

	gui_palette_set(PAL_CLRS_32);

	gui_style_init();

	gui_inpt = (GUI_INPUT) { 0 };
	gui_inpt.drag_delta_min = 5;

	// TODO: CHECK ON ANDROID
	//hal_gesture_enabled(GESTURE_TAP | GESTURE_DOUBLETAP | GESTURE_HOLD | GESTURE_DRAG | GESTURE_PINCH_IN | GESTURE_PINCH_OUT);
}
//--------------------------------------------------------------------------------------
bool gui_input_is_processed(void)
{
	return gui_inpt.is_processed;
}

void gui_input_set_processed(bool is_processed)
{
	gui_inpt.is_processed = is_processed;
}

GUI_ELEM* gui_input_elm_pressed(void) { return gui_inpt.elm_pressed; }
GUI_ELEM* gui_input_elm_released(void) { return gui_inpt.elm_released; }
GUI_ELEM* gui_input_elm_focused(void) { return gui_inpt.elm_focused; }

void gui_input_set_elm_pressed(GUI_ELEM* elm) { gui_inpt.elm_pressed = elm; }
void gui_input_set_elm_released(GUI_ELEM* elm) { gui_inpt.elm_released = elm; }
void gui_input_set_elm_focused(GUI_ELEM* elm) { gui_inpt.elm_focused = elm; }

bool gui_input_is_pressed(void)
{
	return gui_inpt.is_pressed;
}

int32 gui_input_pressed_frames(void)
{
	return gui_inpt.pressed_frames;
}

bool gui_input_is_released(void)
{
	return gui_inpt.is_released;
}

bool gui_input_is_drag(void)
{
	return gui_inpt.is_drag;
}

p32 gui_input_pos(void)
{
	return gui_inpt.pos;
}

p32 gui_input_pos_start(void)
{
	return gui_inpt.pos_start;
}

p32 gui_input_drag_delta(void)
{
	return gui_inpt.drag_delta;
}

void gui_input_process(void)
{
	//----------------------------------------------------------------------------------
	p32 cur_pos = hal_mouse_pos();

	cur_pos.x = (cur_pos.x - game_scr_border_width() * game_scr_scale()) / game_scr_scale();
	cur_pos.y = (cur_pos.y - game_scr_border_height() * game_scr_scale()) / game_scr_scale();

	gui_inpt.pos_last = gui_inpt.pos;
	gui_inpt.pos = cur_pos;

	if(gui_inpt.is_released)
	{
		gui_input_set_elm_pressed(NULL);
		gui_input_set_elm_released(NULL);
		gui_inpt.is_released = false;
	}

	if(hal_mouse_button_down(MOUSE_LEFT_BUTTON))
	{
		if(!gui_inpt.is_pressed)
		{
			gui_inpt.pos_start = gui_inpt.pos_last = cur_pos;
			gui_inpt.is_pressed = true;
			gui_inpt.pressed_frames = 0;
		}

		gui_inpt.pressed_frames++;
		gui_inpt.drag_delta = p32_sub(gui_inpt.pos, gui_inpt.pos_last);
		gui_inpt.is_drag = p32_is_mod_greater_one(gui_inpt.drag_delta, gui_inpt.drag_delta_min);
		gui_inpt.is_released = false;
	}

	if(!hal_mouse_button_down(MOUSE_LEFT_BUTTON))
	{
		if(gui_inpt.is_pressed)
			gui_inpt.pressed_frames++;

		gui_inpt.is_released = gui_inpt.is_pressed;
		gui_inpt.is_pressed = false;
		gui_inpt.is_drag = false;
		gui_inpt.drag_delta = (p32) { 0 };
	}
	//----------------------------------------------------------------------------------
	gui_input_set_processed(false);
	//----------------------------------------------------------------------------------
}
//--------------------------------------------------------------------------------------
uint32* gui_palette_get(void)
{
	return gui_palette;
}

void gui_palette_set(uint32* pal)
{
	memcpy(gui_palette, pal, sizeof(gui_palette));
}

uint32 gui_color(uint32 pal_indx)
{
	return gui_palette[pal_indx];
}
//--------------------------------------------------------------------------------------
GUI_STYLE* gui_style_get(GUI_STYLE_ENUM style_id)
{
	return &gui_styles[style_id];
}

void gui_style_set(GUI_STYLE_ENUM style_id, GUI_STYLE style)
{
	memcpy(&gui_styles[style_id], &style, sizeof(GUI_STYLE));
}
//--------------------------------------------------------------------------------------
void gui_add_display(GUI_DISPLAY* dsp)
{
	vptr_elm_add(&vptr_displays, dsp);
}

void gui_remove_display(GUI_DISPLAY* dsp)
{
	int32 indx = vptr_elm_indx(&vptr_displays, dsp);

	if(indx != -1)
		vptr_elm_delete(&vptr_displays, indx, 1);
}

GUI_DISPLAY* gui_top_display(void)
{
	GUI_DISPLAY* dsp = vptr_elm(&vptr_displays, vptr_displays.count - 1);
	return dsp;
}
//--------------------------------------------------------------------------------------
void gui_update(void)
{
	gui_kb_update();

	gui_input_process();

	for(int32 i = 0; i < vptr_displays.count; i++)
	{
		GUI_DISPLAY* dsp = vptr_elm(&vptr_displays, vptr_displays.count - 1 - i);
		dsp->on_input(dsp);

		if(gui_input_is_processed())
			break;
	}

	for(int32 i = 0; i < vptr_displays.count; i++)
	{
		GUI_DISPLAY* dsp = vptr_elm(&vptr_displays, i);
		dsp->on_update(dsp);
		dsp->calc_size(dsp);
		dsp->calc_pos(dsp);
	}
}
//--------------------------------------------------------------------------------------
void gui_draw(void)
{
	for(int32 i = 0; i < vptr_displays.count; i++)
	{
		GUI_DISPLAY* dsp = vptr_elm(&vptr_displays, i);
		dsp->show(dsp);
	}

	if(1)
	{
		static int released_count = 0;
		if(gui_inpt.is_released)
			released_count++;

		static float mouse_wheel = 0;
		if(hal_mouse_wheel() != 0.f)
			mouse_wheel = hal_mouse_wheel();

		static int elm_released_count = 0;

		if(gui_inpt.elm_released)
			elm_released_count++;

		if(false)
		{
			rd2_font_print(game_font_get(GAME_FONT_ID_MICROFONT_BORDER),
				txt_format("released:%d drag:%s start: %d,%d drag: %d,%d wheel: %0.2f\nelm_rel:%d %s",
					released_count,
					gui_inpt.is_drag ? "TRUE" : "FALSE",
					gui_inpt.pos_start.x, gui_inpt.pos_start.y,
					gui_inpt.drag_delta.x, gui_inpt.drag_delta.y,
					mouse_wheel,
					elm_released_count,
					gui_inpt.elm_pressed ? "elm_pressed" : ""
				),
				0, 0, 1, gui_palette_get()[PAL_TRANSPARENT], gui_palette_get()[PAL_WHITE], FNT_FLG_DRAW);
		}

		rd2_bubble(gui_inpt.pos.x - 1, gui_inpt.pos.y - 1, 3, 3, gui_color(PAL_AQUAMARINE), gui_color(PAL_BLUE));
	}
}
//--------------------------------------------------------------------------------------
