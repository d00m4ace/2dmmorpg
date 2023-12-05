//--------------------------------------------------------------------------------------
RD2SPR_SET spr_set_init(void)
{
	RD2SPR_SET spr_set;

	int32 s = sizeof(RD2SPR_SET);

	c_vec_init(&spr_set.vec_spr_imgs);
	c_vec_init(&spr_set.vec_spr_anims);

	return spr_set;
}

void spr_set_free(RD2SPR_SET* spr_set)
{
	c_vec_foreach(&spr_set->vec_spr_imgs, RD2IMG img)
		rd2_img_free(&img);

	c_vec_deinit(&spr_set->vec_spr_imgs);
	c_vec_deinit(&spr_set->vec_spr_anims);
}

void spr_set_img_push(RD2SPR_SET* spr_set, RD2IMG img)
{
	c_vec_push(&spr_set->vec_spr_imgs, img);
}

int32 spr_set_img_count(RD2SPR_SET* spr_set)
{
	return spr_set->vec_spr_imgs.count;
}

RD2IMG* spr_set_img_get(RD2SPR_SET* spr_set, int32 img_id)
{
	return c_vec_get_ptr(&spr_set->vec_spr_imgs, img_id);
}


void spr_set_anim_push(RD2SPR_SET* spr_set, RD2SPR_ANIM spr_anim)
{
	c_vec_push(&spr_set->vec_spr_anims, spr_anim);
}

int32 spr_set_anim_count(RD2SPR_SET* spr_set)
{
	return spr_set->vec_spr_anims.count;
}

RD2SPR_ANIM* spr_set_anim_get(RD2SPR_SET* spr_set, int32 anim_id)
{
	return c_vec_get_ptr(&spr_set->vec_spr_anims, anim_id);
}
//--------------------------------------------------------------------------------------
