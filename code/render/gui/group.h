//--------------------------------------------------------------------------------------
typedef struct GUI_GROUP
{
	VPTR vptr_elems;

	void* data;

	GUI_ELEM* elm_pressed;
	GUI_ELEM* elm_released;
	GUI_ELEM* elm_focused;
} GUI_GROUP;
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_group(void);
//--------------------------------------------------------------------------------------
void gui_group_add_elm(GUI_ELEM* grp, GUI_ELEM* elm);
void gui_group_remove_elm(GUI_ELEM* grp, GUI_ELEM* elm);
GUI_ELEM* gui_group_get_elm(GUI_ELEM* grp, int32 indx);
int32 gui_group_elems_count(GUI_ELEM* grp);
//--------------------------------------------------------------------------------------
VPTR* gui_group_get_vptr_elems(GUI_ELEM* grp);
//--------------------------------------------------------------------------------------
void gui_group_calc_size(GUI_ELEM* grp);
void gui_group_calc_pos(GUI_ELEM* grp, p32 offs);
void gui_group_show(GUI_ELEM* grp, p32 offs);
void gui_group_on_input(GUI_ELEM* grp);
void gui_group_on_update(GUI_ELEM* grp);
void gui_group_on_free(GUI_ELEM* grp);
//--------------------------------------------------------------------------------------
void gui_group_tile(GUI_ELEM* grp);
//--------------------------------------------------------------------------------------
GUI_ELEM* __gui_group_combo__(GUI_ELEM* elms, ...);
#define gui_group_combo(...) __gui_group_combo__(NULL, __VA_ARGS__, NULL)
//--------------------------------------------------------------------------------------
