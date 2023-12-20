#define GUI_TEXT_INPUT_TXT_MAX_SIZE 256

#define GUI_TEXT_INPUT_FLG_NONE (0U)
#define GUI_TEXT_INPUT_FLG_SHOW_CURSOR (1U<<0)
#define GUI_TEXT_INPUT_FLG_PASSWORD (1U<<1)

typedef struct GUI_TEXT_INPUT
{
	char txt[GUI_TEXT_INPUT_TXT_MAX_SIZE];
	int32 cur_pos;
	int32 x_offs;
	uint32 flags;
} GUI_TEXT_INPUT;

//--------------------------------------------------------------------------------------
GUI_ELEM* gui_text_input(const char* txt);
GUI_ELEM* gui_text_input_password(const char* txt);
//--------------------------------------------------------------------------------------
void gui_text_input_show(GUI_ELEM* elm, p32 offs);
void gui_text_input_on_input(GUI_ELEM* elm);
void gui_text_input_on_update(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_text_input_set_txt(GUI_ELEM* elm, const char* txt);
const char* gui_text_input_get_txt(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
p32 gui_text_input_calc_content_size(GUI_ELEM* elm, int32 cur_pos);
void gui_text_input_move_cursor_to(GUI_ELEM* elm, int32 pos_x);
//--------------------------------------------------------------------------------------
void gui_text_input_add_char(GUI_ELEM* elm, char key);
void gui_text_input_move_cursor(GUI_ELEM* elm, int32 offs);
void gui_text_input_cmd_delete(GUI_ELEM* elm);
void gui_text_input_cmd_backspace(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
