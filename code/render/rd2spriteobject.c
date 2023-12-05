//--------------------------------------------------------------------------------------
RD2SPR_OBJ spr_obj_init(int16 spr_set_id, int16 spr_anim_id)
{
	RD2SPR_OBJ spr_obj;

	int32 s = sizeof(RD2SPR_OBJ);

	spr_obj.spr_set_id = spr_set_id;
	spr_obj.spr_anim_id = spr_anim_id;
	spr_obj.frame_id = 0;
	spr_obj.frames = 0;
	spr_obj.replace_colors_id = -1;
	spr_obj.flags = SPR_FLG_NONE;

	return spr_obj;
}

int32 spr_obj_frame_id_get(RD2SPR_OBJ* spr_obj)
{
	return spr_obj->frame_id;
}

RD2SPR_SET* spr_obj_spr_set_get(RD2SPR_OBJ* spr_obj)
{
	return spr_man_spr_sets_get(spr_obj->spr_set_id);
}

RD2SPR_ANIM* spr_obj_spr_anim_get(RD2SPR_OBJ* spr_obj)
{
	RD2SPR_SET* spr_set = spr_man_spr_sets_get(spr_obj->spr_set_id);
	return spr_set_anim_get(spr_set, spr_obj->spr_anim_id);
}

RD2SPR_FRAME* spr_obj_spr_frame_get(RD2SPR_OBJ* spr_obj)
{
	RD2SPR_SET* spr_set = spr_man_spr_sets_get(spr_obj->spr_set_id);
	RD2SPR_ANIM* spr_set_anim = spr_set_anim_get(spr_set, spr_obj->spr_anim_id);
	return spr_anim_frame_get(spr_set_anim, spr_obj->frame_id);
}

RD2IMG* spr_obj_spr_frame_img_get(RD2SPR_OBJ* spr_obj)
{
	RD2SPR_SET* spr_set = spr_man_spr_sets_get(spr_obj->spr_set_id);
	RD2SPR_ANIM* spr_set_anim = spr_set_anim_get(spr_set, spr_obj->spr_anim_id);
	RD2SPR_FRAME* spr_frame = spr_anim_frame_get(spr_set_anim, spr_obj->frame_id);
	return spr_set_img_get(spr_set, spr_frame->img_id);
}

void spr_obj_update(RD2SPR_OBJ* spr_obj)
{
	RD2SPR_SET* spr_set = spr_man_spr_sets_get(spr_obj->spr_set_id);
	RD2SPR_ANIM* spr_set_anim = spr_set_anim_get(spr_set, spr_obj->spr_anim_id);

	if(spr_obj->flags & SPR_FLG_ANIM_LOOP_BACK)
		spr_obj->frames--;
	else
		spr_obj->frames++;

	if(spr_obj->frames < 0)
	{
		spr_obj->flags &= ~SPR_FLG_ANIM_LOOP_BACK;
		spr_obj->frames = 0;
	}

	int32 frames = 0;
	int32 frame_id = -1;
	for(int32 i = 0; i < spr_set_anim->vec_spr_frames.count; i++)
	{
		frames += c_vec_get_ptr(&spr_set_anim->vec_spr_frames, i)->frames;

		if(spr_obj->frames < frames)
		{
			frame_id = i;
			break;
		}
	}

	spr_obj->frame_id = frame_id;

	if(frame_id == -1)
	{
		if(spr_set_anim->flags & SPR_ANIM_FLG_LOOP_BACK)
		{
			spr_obj->flags |= SPR_FLG_ANIM_LOOP_BACK;
			spr_obj->frames = frames;
			spr_obj->frame_id = spr_set_anim->vec_spr_frames.count - 1;
		}
		else if(spr_set_anim->flags & SPR_ANIM_FLG_LOOP)
		{
			spr_obj->frames = 0;
			spr_obj->frame_id = 0;
		}
		else if(spr_set_anim->flags & SPR_ANIM_FLG_ONCE)
		{
			spr_obj->frames = frames;
			spr_obj->frame_id = spr_set_anim->vec_spr_frames.count - 1;
			spr_obj->flags |= SPR_FLG_ANIM_END;
		}
		else
		{
			spr_obj->frame_id = 0;
			spr_obj->flags |= SPR_FLG_ANIM_END;
		}
	}
}

#define RD2_SPR_OBJ_MAX_IMG_SIZE 256

RD2IMG spr_obj_render(RD2SPR_OBJ* spr_obj)
{
	static RD2IMG spr_img = { 0 }, spr_tmp = { 0 };

	if(spr_img.pixels32 == NULL)
	{
		spr_img = rd2_img_init(RD2_SPR_OBJ_MAX_IMG_SIZE, RD2_SPR_OBJ_MAX_IMG_SIZE);
		spr_tmp = rd2_img_init(RD2_SPR_OBJ_MAX_IMG_SIZE, RD2_SPR_OBJ_MAX_IMG_SIZE);
	}

	RD2SPR_SET* spr_set = spr_man_spr_sets_get(spr_obj->spr_set_id);
	RD2SPR_ANIM* spr_set_anim = spr_set_anim_get(spr_set, spr_obj->spr_anim_id);
	RD2SPR_FRAME* spr_frame = spr_anim_frame_get(spr_set_anim, spr_obj->frame_id);
	RD2IMG* spr_obj_frame_img = spr_set_img_get(spr_set, spr_frame->img_id);

	spr_img.width = spr_tmp.width = spr_obj_frame_img->width;
	spr_img.height = spr_tmp.height = spr_obj_frame_img->height;

	rd2_img_copy(&spr_img, spr_obj_frame_img);

	if(spr_obj->replace_colors_id != -1)
	{
		RD2_REPLACE_COLORS* rep_col = spr_man_replace_colors_get(spr_obj->replace_colors_id);
		rd2_img_color_replace(&spr_img, rep_col->rd2c_ori.values, rep_col->rd2c_rep.values, rep_col->rd2c_ori.count);
	}

	if(spr_obj->flags & SPR_FLG_FLIP_H)
		rd2_img_hflip(&spr_img);

	if(spr_obj->flags & SPR_FLG_FLIP_V)
		rd2_img_vflip(&spr_img);

	if(spr_obj->flags & SPR_FLG_ROT_90)
	{
		rd2_img_rotate90(&spr_tmp, &spr_img);
		uint32* pixels32;
		SWAP(spr_img.pixels32, spr_tmp.pixels32, pixels32);
	}

	if(spr_obj->flags & SPR_FLG_ROT_180)
	{
		rd2_img_hflip(&spr_img);
		rd2_img_vflip(&spr_img);
	}

	return spr_img;
}

void spr_obj_draw(RD2SPR_OBJ* spr_obj, p32 pos)
{
	RD2IMG spr_img = spr_obj_render(spr_obj);

	rd2_spr(&spr_img, pos.x, pos.y);
}

void spr_obj_spr_anim_set(RD2SPR_OBJ* spr_obj, int32 anim_id)
{
	spr_obj->spr_anim_id = anim_id;
	spr_obj->frame_id = 0;
	spr_obj->frames = 0;

	spr_obj->flags &= ~SPR_FLG_ANIM_END;
	spr_obj->flags &= ~SPR_FLG_ANIM_LOOP_BACK;
}
//--------------------------------------------------------------------------------------
