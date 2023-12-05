typedef struct RD2_REPLACE_COLORS
{
	c_vec_uint32 rd2c_ori;
	c_vec_uint32 rd2c_rep;
} RD2_REPLACE_COLORS;

typedef struct c_vec_of(RD2_REPLACE_COLORS) c_vec_replace_colors;

RD2_REPLACE_COLORS replace_colors_init(void);
void replace_colors_free(RD2_REPLACE_COLORS* spr_rep_col);

void replace_colors_push(RD2_REPLACE_COLORS* spr_rep_col, rd2c origin_color, rd2c replace_color);
int32 replace_colors_count(RD2_REPLACE_COLORS* spr_rep_col);
p32 replace_colors_get(RD2_REPLACE_COLORS* spr_rep_col, int32 col_id);