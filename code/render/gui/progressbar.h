#define GUI_PROGRESSBAR_TXT_MAX_SIZE 128

typedef struct GUI_PROGRESSBAR
{
	char txt[GUI_PROGRESSBAR_TXT_MAX_SIZE];

	int32 val, val_min, val_max;
} GUI_PROGRESSBAR;

//--------------------------------------------------------------------------------------
GUI_ELEM* gui_progressbar(const char* txt, int32 val, int32 val_min, int32 val_max);
GUI_ELEM* gui_progressbar_fixed_size(const char* txt, int32 val, int32 val_min, int32 val_max, p32 fixed_size);
//--------------------------------------------------------------------------------------
void gui_progressbar_set_txt(GUI_ELEM* elm, const char* txt);

void gui_progressbar_set_val(GUI_ELEM* elm, int32 val);
void gui_progressbar_add_val(GUI_ELEM* elm, int32 val);

int32 gui_progressbar_get_val(GUI_ELEM* elm);
int32 gui_progressbar_get_val_min(GUI_ELEM* elm);
int32 gui_progressbar_get_val_max(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_progressbar_show(GUI_ELEM* elm, p32 offs);
void gui_progressbar_on_input(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
