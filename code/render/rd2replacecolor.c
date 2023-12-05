RD2_REPLACE_COLORS replace_colors_init(void)
{
	RD2_REPLACE_COLORS spr_rep_col;

	int32 s = sizeof(RD2_REPLACE_COLORS);

	c_vec_init(&spr_rep_col.rd2c_ori);
	c_vec_init(&spr_rep_col.rd2c_rep);

	return spr_rep_col;
}

void replace_colors_free(RD2_REPLACE_COLORS* spr_rep_col)
{
	c_vec_deinit(&spr_rep_col->rd2c_ori);
	c_vec_deinit(&spr_rep_col->rd2c_rep);
}

void replace_colors_push(RD2_REPLACE_COLORS* spr_rep_col, rd2c origin_color, rd2c replace_color)
{
	c_vec_push(&spr_rep_col->rd2c_ori, origin_color);
	c_vec_push(&spr_rep_col->rd2c_rep, replace_color);
}

int32 replace_colors_count(RD2_REPLACE_COLORS* spr_rep_col)
{
	return spr_rep_col->rd2c_ori.count;
}

p32 replace_colors_get(RD2_REPLACE_COLORS* spr_rep_col, int32 col_id)
{
	return (p32) { c_vec_get(&spr_rep_col->rd2c_ori, col_id), c_vec_get(&spr_rep_col->rd2c_rep, col_id) };
}
