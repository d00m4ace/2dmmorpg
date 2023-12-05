//--------------------------------------------------------------------------------------
GUI_ELEM* gui_toggle(void);
//--------------------------------------------------------------------------------------
GUI_ELEM* __gui_toggle_combo__(char* str0, ...);
#define gui_toggle_combo(...) __gui_toggle_combo__(NULL, __VA_ARGS__, NULL)
//--------------------------------------------------------------------------------------
void gui_toggle_add(GUI_ELEM* tgl, GUI_ELEM* elm);
GUI_ELEM* gui_toggle_get_elm(GUI_ELEM* tgl, int32 indx);
//--------------------------------------------------------------------------------------
int32 gui_toggle_elems_count(GUI_ELEM* tgl);
int32 gui_toggle_get_selected(GUI_ELEM* tgl);
void gui_toggle_set_selected(GUI_ELEM* tgl, int32 selected);
//--------------------------------------------------------------------------------------
void gui_toggle_set_fixed_size(GUI_ELEM* tgl, p32 fixed_size);
//--------------------------------------------------------------------------------------
void gui_toggle_on_input(GUI_ELEM* tgl);
//--------------------------------------------------------------------------------------
