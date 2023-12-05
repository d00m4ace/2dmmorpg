typedef struct GUI_SPINNER
{
	GUI_ELEM* progbar;
	GUI_ELEM* btn_inc;
	GUI_ELEM* btn_dec;
} GUI_SPINNER;

//--------------------------------------------------------------------------------------
GUI_ELEM* gui_spinner(const char* txt, int32 val, int32 val_min, int32 val_max);
//--------------------------------------------------------------------------------------
void gui_spinner_set_txt(GUI_ELEM* elm, const char* txt);

void gui_spinner_set_val(GUI_ELEM* elm, int32 val);
void gui_spinner_add_val(GUI_ELEM* elm, int32 val);

int32 gui_spinner_get_val(GUI_ELEM* elm);
int32 gui_spinner_get_val_min(GUI_ELEM* elm);
int32 gui_spinner_get_val_max(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_spinner_on_input(GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
