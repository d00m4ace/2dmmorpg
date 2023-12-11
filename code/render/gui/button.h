#define GUI_BUTTON_TXT_MAX_SIZE 1024

typedef struct GUI_BUTTON
{
	char txt[GUI_BUTTON_TXT_MAX_SIZE];
} GUI_BUTTON;

//--------------------------------------------------------------------------------------
GUI_ELEM* gui_button(const char* txt);
GUI_ELEM* gui_button_fixed_size(const char* txt, p32 fixed_size);
GUI_ELEM* gui_button_size(const char* txt, p32 min_size);
GUI_ELEM* gui_button_size_word_wrap(const char* txt, p32 max_size);
GUI_ELEM* gui_button_style(const char* txt, GUI_STYLE_ENUM style);

void gui_button_set_txt(GUI_ELEM* elm, const char* txt);
const char* gui_button_get_txt(GUI_ELEM* elm);

void gui_button_calc_size(GUI_ELEM* elm);
void gui_button_show(GUI_ELEM* elm, p32 offs);
//--------------------------------------------------------------------------------------
