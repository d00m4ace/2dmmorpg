//--------------------------------------------------------------------------------------
#include "guienums.h"
//--------------------------------------------------------------------------------------
typedef struct GUI_DISPLAY GUI_DISPLAY;
typedef struct GUI_PLANE GUI_PLANE;
typedef struct GUI_ELEM GUI_ELEM;
typedef struct GUI_STYLE GUI_STYLE;
//--------------------------------------------------------------------------------------
void gui_init(void);
//--------------------------------------------------------------------------------------
uint32* gui_palette_get(void);
void gui_palette_set(uint32* pal);
// get color by palette index, see PALETTE_COLORS
uint32 gui_color(uint32 pal_indx);
//--------------------------------------------------------------------------------------
GUI_STYLE* gui_style_get(GUI_STYLE_ENUM style_id);
void gui_style_set(GUI_STYLE_ENUM style_id, GUI_STYLE style);
//--------------------------------------------------------------------------------------
void gui_input_process(void);
bool gui_input_is_processed(void);
void gui_input_set_processed(bool is_processed);
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_input_elm_pressed(void);
GUI_ELEM* gui_input_elm_released(void);
GUI_ELEM* gui_input_elm_focused(void);

void gui_input_set_elm_pressed(GUI_ELEM* elm);
void gui_input_set_elm_released(GUI_ELEM* elm);
void gui_input_set_elm_focused(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
bool gui_input_is_pressed(void);
bool gui_input_is_released(void);
bool gui_input_is_drag(void);

int32 gui_input_pressed_frames(void);

p32 gui_input_pos(void);
p32 gui_input_pos_start(void);
p32 gui_input_drag_delta(void);
//--------------------------------------------------------------------------------------
void gui_add_display(GUI_DISPLAY* dsp);
void gui_remove_display(GUI_DISPLAY* dsp);
GUI_DISPLAY* gui_top_display(void);
//--------------------------------------------------------------------------------------
void gui_update(void);
void gui_draw(void);
//--------------------------------------------------------------------------------------
#include "element.h"
#include "group.h"
#include "guistyles.h"
#include "scrolling.h"
#include "plane.h"
#include "display.h"
#include "text.h"
#include "button.h"
#include "space.h"
#include "progressbar.h"
#include "spinner.h"
#include "composite.h"
#include "tabs.h"
#include "toggle.h"
#include "textinput.h"
#include "list.h"
#include "layouts.h"
#include "helper.h"
#include "keyboard.h"
//--------------------------------------------------------------------------------------
