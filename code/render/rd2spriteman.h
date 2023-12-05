void spr_man_init(void);
void spr_man_free(void);

void spr_man_replace_colors_push(RD2_REPLACE_COLORS* spr_rep_col);
int32 spr_man_replace_colors_count(void);
RD2_REPLACE_COLORS* spr_man_replace_colors_get(int32 rep_col_id);

void spr_man_spr_set_push(RD2SPR_SET* spr_set);
int32 spr_man_spr_sets_count(void);
RD2SPR_SET* spr_man_spr_sets_get(int32 spr_sets_id);

void spr_man_spr_obj_push(RD2SPR_OBJ* spr_obj);
int32 spr_man_spr_obj_count(void);
RD2SPR_OBJ* spr_man_spr_obj_get(int32 spr_obj_id);

void spr_man_spr_objs_update(void);