//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite(void);
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_tr(const char* txt, const char* txt_tr, p32 fixed_size);
GUI_ELEM* gui_composite_br(const char* txt, const char* txt_br, p32 fixed_size);
GUI_ELEM* gui_composite_tr_br(const char* txt, const char* txt_tr, const char* txt_br, p32 fixed_size);
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_combo(const char* txt, const char* txt_tl, const char* txt_tr, const char* txt_bl, const char* txt_br);
void gui_composite_set_fixed_size(GUI_ELEM* cmps, p32 fixed_size);
//--------------------------------------------------------------------------------------
void gui_composite_add(GUI_ELEM* cmps, GUI_ELEM* elm);
//--------------------------------------------------------------------------------------
void gui_composite_calc_size(GUI_ELEM* cmps);
//--------------------------------------------------------------------------------------
GUI_ELEM* gui_composite_get_elm(GUI_ELEM* cmps, int32 indx);
void gui_composite_set_txt(GUI_ELEM* cmps, int32 indx, const char* txt);
//--------------------------------------------------------------------------------------
