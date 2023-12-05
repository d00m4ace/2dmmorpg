void game_imgs_load_all(void)
{
	game_imgs_load("imgs/img_i16.png", 16, 0, IMG_I16_ENUMS_COUNT - IMG_I16_ENUMS_OFFSET);

	game_imgs_load("imgs/img_chrs_head.png", 24, IMG_CHRS_HEAD_ENUMS_OFFSET, IMG_CHRS_HEAD_ENUMS_COUNT - IMG_CHRS_HEAD_ENUMS_OFFSET);
	game_imgs_load("imgs/img_chrs_face.png", 24, IMG_CHRS_FACE_ENUMS_OFFSET, IMG_CHRS_FACE_ENUMS_COUNT - IMG_CHRS_FACE_ENUMS_OFFSET);
	game_imgs_load("imgs/img_chrs_hair.png", 24, IMG_CHRS_HAIR_ENUMS_OFFSET, IMG_CHRS_HAIR_ENUMS_COUNT - IMG_CHRS_HAIR_ENUMS_OFFSET);
	game_imgs_load("imgs/img_chrs_beard.png", 24, IMG_CHRS_BEARD_ENUMS_OFFSET, IMG_CHRS_BEARD_ENUMS_COUNT - IMG_CHRS_BEARD_ENUMS_OFFSET);
	game_imgs_load("imgs/img_chrs_helm.png", 24, IMG_CHRS_HELM_ENUMS_OFFSET, IMG_CHRS_HELM_ENUMS_COUNT - IMG_CHRS_HELM_ENUMS_OFFSET);
	game_imgs_load("imgs/img_chrs_body.png", 24, IMG_CHRS_BODY_ENUMS_OFFSET, IMG_CHRS_BODY_ENUMS_COUNT - IMG_CHRS_BODY_ENUMS_OFFSET);

	game_imgs_load("imgs/img_mobs_16.png", 16, IMG_MOBS_16_ENUMS_OFFSET, IMG_MOBS_16_ENUMS_COUNT - IMG_MOBS_16_ENUMS_OFFSET);
	game_imgs_load("imgs/img_mobs_24.png", 24, IMG_MOBS_24_ENUMS_OFFSET, IMG_MOBS_24_ENUMS_COUNT - IMG_MOBS_24_ENUMS_OFFSET);
	game_imgs_load("imgs/img_mobs_32.png", 32, IMG_MOBS_32_ENUMS_OFFSET, IMG_MOBS_32_ENUMS_COUNT - IMG_MOBS_32_ENUMS_OFFSET);
	game_imgs_load("imgs/img_mobs_40.png", 40, IMG_MOBS_40_ENUMS_OFFSET, IMG_MOBS_40_ENUMS_COUNT - IMG_MOBS_40_ENUMS_OFFSET);
	game_imgs_load("imgs/img_mobs_48.png", 48, IMG_MOBS_48_ENUMS_OFFSET, IMG_MOBS_48_ENUMS_COUNT - IMG_MOBS_48_ENUMS_OFFSET);
}
