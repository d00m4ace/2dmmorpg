//--------------------------------------------------------------------------------------
GUI_ELEM* gui_tabs(void);
//--------------------------------------------------------------------------------------
GUI_ELEM* __gui_tabs_combo__(char* str0, ...);
#define gui_tabs_combo(...) __gui_tabs_combo__(NULL, __VA_ARGS__, NULL)
//--------------------------------------------------------------------------------------
void gui_tabs_add(GUI_ELEM* tbs, GUI_ELEM* elm);
GUI_ELEM* gui_tabs_get_elm(GUI_ELEM* tbs, int32 indx);
//--------------------------------------------------------------------------------------
void gui_tabs_align(GUI_ELEM* tbs, GUI_ALIGN_ENUM align);
//--------------------------------------------------------------------------------------
int32 gui_tabs_elems_count(GUI_ELEM* tbs);
int32 gui_tabs_get_selected(GUI_ELEM* tbs);
void gui_tabs_set_selected(GUI_ELEM* tbs, int32 selected);
//--------------------------------------------------------------------------------------
void gui_tabs_set_fixed_size(GUI_ELEM* tbs, p32 fixed_size);
//--------------------------------------------------------------------------------------
void gui_tabs_on_input(GUI_ELEM* tbs);
//--------------------------------------------------------------------------------------
