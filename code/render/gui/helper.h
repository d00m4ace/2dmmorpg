void gui_helper_elems_rest_flag(VPTR* vptr_elems, uint32 flag);
void gui_helper_elems_add_flag(VPTR* vptr_elems, uint32 flag);
void gui_helper_elems_set_flags(VPTR* vptr_elems, uint32 flags);

int32 gui_helper_elems_find_first_set_flags(VPTR* vptr_elems, uint32 flags);
int32 gui_helper_elems_find_first_set_no_flags(VPTR* vptr_elems, uint32 flags);

void gui_helper_elems_rest_pln(VPTR* vptr_elems);
void gui_helper_elems_rest_gui_state(VPTR* vptr_elems);

void gui_helper_elems_set_pln(VPTR* vptr_elems, GUI_PLANE* pln);

void gui_helper_elems_rest_origin(VPTR* vptr_elems);
void gui_helper_elems_add_origin(VPTR* vptr_elems, p32 offs);

void gui_helper_elems_rest_pos(VPTR* vptr_elems);
void gui_helper_elems_set_size(VPTR* vptr_elems, p32 size);

void gui_helper_elems_set_calc_size(VPTR* vptr_elems, void (*calc_size)(GUI_ELEM* elm));
void gui_helper_elems_set_calc_pos(VPTR* vptr_elems, void (*calc_pos)(GUI_ELEM* elm, p32 offs));
void gui_helper_elems_set_show(VPTR* vptr_elems, void (*show)(GUI_ELEM* elm, p32 offs));
void gui_helper_elems_set_on_update(VPTR* vptr_elems, void (*on_update)(GUI_ELEM* elm));
void gui_helper_elems_set_on_input(VPTR* vptr_elems, void (*on_input)(GUI_ELEM* elm));
void gui_helper_elems_set_on_free(VPTR* vptr_elems, void (*on_free)(GUI_ELEM* elm));

void gui_helper_elems_calc_size(VPTR* vptr_elems);
void gui_helper_elems_calc_pos(VPTR* vptr_elems, p32 offs);
void gui_helper_elems_show(VPTR* vptr_elems, p32 offs);
void gui_helper_elems_on_update(VPTR* vptr_elems);
void gui_helper_elems_on_input(VPTR* vptr_elems);
void gui_helper_elems_on_free(VPTR* vptr_elems);

aabb32 gui_helper_elems_calc_max_aabb(VPTR* vptr_elems);

aabb32 gui_helper_clip_aabb32(aabb32 view, aabb32 target);

p32 gui_helper_txt_calc_content_size(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state);
p32 gui_helper_txt_calc_content_size_word_wrap(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state, p32 max_size);

p32 gui_helper_str_calc_content_size(const char* txt, GUI_STYLE* style, GUI_ELEM_STATE_ENUM gui_state);