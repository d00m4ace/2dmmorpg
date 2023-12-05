RD2SPR_ANIM spr_anim_init(uint32 flags, int32 frames_count, RD2SPR_FRAME frame[])
{
	RD2SPR_ANIM spr_anim;

	int32 s = sizeof(RD2SPR_ANIM);

	c_vec_init(&spr_anim.vec_spr_frames);

	for(int32 i = 0; i < frames_count; i++)
		c_vec_push(&spr_anim.vec_spr_frames, frame[i]);

	spr_anim.flags = flags;

	return spr_anim;
}

void spr_anim_free(RD2SPR_ANIM* spr_anim)
{
	c_vec_deinit(&spr_anim->vec_spr_frames);
}

void spr_anim_frame_push(RD2SPR_ANIM* spr_anim, RD2SPR_FRAME frame)
{
	c_vec_push(&spr_anim->vec_spr_frames, frame);
}

int32 spr_anim_frame_count(RD2SPR_ANIM* spr_anim)
{
	return spr_anim->vec_spr_frames.count;
}

RD2SPR_FRAME* spr_anim_frame_get(RD2SPR_ANIM* spr_anim, int32 frame_id)
{
	return c_vec_get_ptr(&spr_anim->vec_spr_frames, frame_id);
}