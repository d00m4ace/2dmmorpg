//--------------------------------------------------------------------------------------
#define GUI_LIST_FLG_NONE (0U)
#define GUI_LIST_FLG_VERTICAL (1U<<0)
#define GUI_LIST_FLG_HORIZONTAL (1U<<1)
//--------------------------------------------------------------------------------------
typedef struct GUI_LIST
{
	VPTR vptr_elems;
	int32 selected;
	uint32 flags;
} GUI_LIST;

//--------------------------------------------------------------------------------------
GUI_ELEM* gui_list(void);
//--------------------------------------------------------------------------------------
void gui_list_set_vertical(GUI_ELEM* lst);
void gui_list_set_horizontal(GUI_ELEM* lst);
//--------------------------------------------------------------------------------------
void gui_list_add_elm(GUI_ELEM* lst, GUI_ELEM* elm);
void gui_list_remove_elm(GUI_ELEM* lst, GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_list_get_elm(GUI_ELEM* lst, int32 indx);
int32 gui_list_elems_count(GUI_ELEM* lst);
//--------------------------------------------------------------------------------------
int32 gui_list_get_selected(GUI_ELEM* lst);
void gui_list_set_selected(GUI_ELEM* lst, int32 selected);
GUI_ELEM* gui_list_get_selected_elm(GUI_ELEM* lst);
int32 gui_list_set_selected_elm(GUI_ELEM* lst, GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_list_calc_size(GUI_ELEM* lst);
void gui_list_calc_pos(GUI_ELEM* lst, p32 offs);
void gui_list_show(GUI_ELEM* lst, p32 offs);
void gui_list_on_input(GUI_ELEM* lst);
void gui_list_on_update(GUI_ELEM* lst);
void gui_list_on_free(GUI_ELEM* lst);
//--------------------------------------------------------------------------------------
