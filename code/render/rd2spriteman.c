static c_vec_spr_sets g_vec_spr_sets;
static c_vec_replace_colors g_vec_replace_colors;
static c_vec_spr_objs g_vec_spr_objs;

void spr_man_init(void)
{
	c_vec_init(&g_vec_spr_sets);
	c_vec_init(&g_vec_replace_colors);
	c_vec_init(&g_vec_spr_objs);

	for(int32 i = 0; i < PALETTE_COLORS_ENUM_COUNT; i++)
	{
		RD2_REPLACE_COLORS spr_rep_col = replace_colors_init();
		replace_colors_push(&spr_rep_col, PAL_CLRS_32[PAL_WHITE], PAL_CLRS_32[i]);

		spr_man_replace_colors_push(&spr_rep_col);

		p32 c = replace_colors_get(spr_man_replace_colors_get(i), 0);
		PRINT("%d : %d %d %d %d > %d %d %d %d", i, ARGB32_GETR(c.ux), ARGB32_GETG(c.ux), ARGB32_GETB(c.ux), ARGB32_GETA(c.ux), ARGB32_GETR(c.uy), ARGB32_GETG(c.uy), ARGB32_GETB(c.uy), ARGB32_GETA(c.uy));
	}
}

void spr_man_free(void)
{

}

void spr_man_replace_colors_push(RD2_REPLACE_COLORS* spr_rep_col)
{
	c_vec_push(&g_vec_replace_colors, *spr_rep_col);
}

int32 spr_man_replace_colors_count(void)
{
	return g_vec_replace_colors.count;
}

RD2_REPLACE_COLORS* spr_man_replace_colors_get(int32 rep_col_id)
{
	return c_vec_get_ptr(&g_vec_replace_colors, rep_col_id);
}


void spr_man_spr_set_push(RD2SPR_SET* spr_set)
{
	c_vec_push(&g_vec_spr_sets, *spr_set);
}

int32 spr_man_spr_sets_count(void)
{
	return g_vec_spr_sets.count;
}

RD2SPR_SET* spr_man_spr_sets_get(int32 spr_sets_id)
{
	return c_vec_get_ptr(&g_vec_spr_sets, spr_sets_id);
}


void spr_man_spr_obj_push(RD2SPR_OBJ* spr_obj)
{
	c_vec_push(&g_vec_spr_objs, *spr_obj);
}

int32 spr_man_spr_obj_count(void)
{
	return g_vec_spr_objs.count;
}

RD2SPR_OBJ* spr_man_spr_obj_get(int32 spr_obj_id)
{
	return c_vec_get_ptr(&g_vec_spr_objs, spr_obj_id);
}

void spr_man_spr_objs_update(void)
{
	c_vec_foreach_ptr(&g_vec_spr_objs, RD2SPR_OBJ * spr_obj)
		spr_obj_update(spr_obj);
}