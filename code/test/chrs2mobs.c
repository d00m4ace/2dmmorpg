//--------------------------------------------------------------------------------------
int32 mob_set_data(int32 mob_offs)
{
	RD2SPR_SET spr_set = spr_set_init();
	int32 spr_set_id = spr_man_spr_sets_count();

	{
		RD2IMG* mob_body_img = game_imgs_get(mob_offs);
		RD2IMG* mob_head_img = game_imgs_get(mob_offs + 1);

		int32 w = mob_body_img->width, h = mob_body_img->height;

		{
			RD2IMG spr_img = rd2_img_init(w, h);

			rd2_img_copy(&spr_img, mob_body_img);
			rd2_img_blit(&spr_img, mob_head_img, 0, 0, 0, 0, w, h);

			spr_set_img_push(&spr_set, spr_img);
		}

		{
			RD2IMG spr_img = rd2_img_init(w, h);

			rd2_img_copy(&spr_img, mob_body_img);
			rd2_img_blit(&spr_img, mob_head_img, 1, 1, 0, 0, w, h);

			spr_set_img_push(&spr_set, spr_img);
		}
	}

	{
		spr_set_anim_push(&spr_set, spr_anim_init(SPR_ANIM_FLG_LOOP, 2, (RD2SPR_FRAME[]) { { 0, 30 }, { 1, 30 } }));
	}

	spr_man_spr_set_push(&spr_set);

	return spr_set_id;
}
//--------------------------------------------------------------------------------------
#define CHAR_IMG_TILE_SIZE 24
#define CHAR_IMG_COUNT 6

int32 char_set_data(void)
{
	RD2SPR_SET spr_set = spr_set_init();
	int32 spr_set_id = spr_man_spr_sets_count();

	for(size_t i = 0; i < CHAR_IMG_COUNT; i++)
	{
		RD2IMG spr_img = rd2_img_init(CHAR_IMG_TILE_SIZE, CHAR_IMG_TILE_SIZE);
		spr_set_img_push(&spr_set, spr_img);
	}

	{
		spr_set_anim_push(&spr_set, spr_anim_init(SPR_ANIM_FLG_LOOP, 2, (RD2SPR_FRAME[]) { { 0, 30 }, { 1, 30 } }));
		spr_set_anim_push(&spr_set, spr_anim_init(SPR_ANIM_FLG_LOOP_BACK, 5, (RD2SPR_FRAME[]) { { 0, 30 }, { 1, 30 }, { 0, 30 }, { 2, 30 }, { 3, 30 } }));
	}

	spr_man_spr_set_push(&spr_set);

	return spr_set_id;
}

void char_add_part(RD2IMG* spr_img, RD2IMG* spr_helper, int32 img_id, int32 pal_color_id, p32 offs, bool flip)
{
	if(img_id == -1)
		return;

	rd2_img_copy(spr_helper, game_imgs_get(img_id));

	rd2c col_rep[] = { ARGB32(255,255,255,255), PAL_CLRS_32[pal_color_id] };

	rd2_img_color_replace(spr_helper, col_rep, col_rep + 1, 1);

	if(flip)
		rd2_img_vflip(spr_helper);

	rd2_img_blit(spr_img, spr_helper, offs.x, offs.y, 0, 0, CHAR_IMG_TILE_SIZE, CHAR_IMG_TILE_SIZE);
}

void char_add_parts(RD2SPR_SET* spr_set_ptr, int32 img_id, RD2IMG* spr_helper, int32 char_parts[6][2], p32 head_offs, bool head_flip)
{
	RD2IMG* spr_img = spr_set_img_get(spr_set_ptr, img_id);

	rd2_img_fill(spr_img, 0, 0, CHAR_IMG_TILE_SIZE, CHAR_IMG_TILE_SIZE, 0);

	char_add_part(spr_img, spr_helper, char_parts[0][0], char_parts[0][1], (p32) { 0, 0 }, false);

	char_add_part(spr_img, spr_helper, char_parts[1][0], char_parts[1][1], head_offs, head_flip);
	char_add_part(spr_img, spr_helper, char_parts[2][0], char_parts[2][1], head_offs, head_flip);
	char_add_part(spr_img, spr_helper, char_parts[3][0], char_parts[3][1], head_offs, head_flip);
	char_add_part(spr_img, spr_helper, char_parts[4][0], char_parts[4][1], head_offs, head_flip);
	char_add_part(spr_img, spr_helper, char_parts[5][0], char_parts[5][1], head_offs, head_flip);
}

void char_set_update(int32 spr_set_id, int32 char_parts[6][2])
{
	RD2SPR_SET* spr_set_ptr = spr_man_spr_sets_get(spr_set_id);

	RD2IMG spr_helper = rd2_img_stack_tmp(CHAR_IMG_TILE_SIZE, CHAR_IMG_TILE_SIZE, 0);

	char_add_parts(spr_set_ptr, 0, &spr_helper, char_parts, (p32) { 0, 0 }, false);
	char_add_parts(spr_set_ptr, 1, &spr_helper, char_parts, (p32) { 1, 1 }, false);

	char_add_parts(spr_set_ptr, 2, &spr_helper, char_parts, (p32) { 0, 0 }, true);
	char_add_parts(spr_set_ptr, 3, &spr_helper, char_parts, (p32) { -1, 1 }, true);
}
//--------------------------------------------------------------------------------------
int main(void)
{
	//--------------------------------------------------------------------------------------
	// Initialization
	//--------------------------------------------------------------------------------------
	hal_system_init();
	game_init();
	//--------------------------------------------------------------------------------------
	// low end android 1280x720
	//static int SCR_WIDTH_MIN = 320;
	//static int SCR_HEIGHT_MIN = 180;
	// hi end android 1080+
	static int SCR_WIDTH_MIN = 422;
	static int SCR_HEIGHT_MIN = 216;
	//--------------------------------------------------------------------------------------
	static int SCR_W_BORDER_MAX = 5;
	static int SCR_H_BORDER_MAX = 0;
#ifdef _WIN32	
	SCR_W_BORDER_MAX = 6;
	SCR_H_BORDER_MAX = 6;
#endif
	game_scr_init(SCR_WIDTH_MIN, SCR_HEIGHT_MIN, SCR_W_BORDER_MAX, SCR_H_BORDER_MAX);
	//--------------------------------------------------------------------------------------
	{
		GUI_DISPLAY* dsp = gui_display(0);
		gui_add_display(dsp);

		int32 w = 320, h = 180;
		dsp->pos = (p32){ (game_scr_width() - w) / 2,(game_scr_height() - h) / 2 };

		//GUI_PLANE* pln = gui_plane(-1, (p32) { w, h });
		//gui_display_add_plane(dsp, pln);
	}

	game_txt_set_lang(TXT_LANG_ID_RU);

	{
		int32 mob_id = mob_set_data(IMG_MOBS_48_ENUMS_OFFSET);

		{
			RD2SPR_OBJ spr_obj = spr_obj_init(mob_id, 0);
			spr_obj.flags = SPR_FLG_ROT_90;

			spr_obj.replace_colors_id = rnd(0, 32) - 1;
			spr_man_spr_obj_push(&spr_obj);
		}

		{
			RD2SPR_OBJ spr_obj = spr_obj_init(mob_id, 0);
			spr_obj.flags = SPR_FLG_ROT_180;
			spr_obj.replace_colors_id = rnd(0, 32) - 1;
			spr_man_spr_obj_push(&spr_obj);
		}

		{
			RD2SPR_OBJ spr_obj = spr_obj_init(mob_id, 0);
			spr_obj.flags = SPR_FLG_ROT_90 | SPR_FLG_ROT_180;
			spr_obj.replace_colors_id = rnd(0, 32) - 1;
			spr_man_spr_obj_push(&spr_obj);
		}
	}

	{
		int32 char_id = char_set_data();

		int32 char_parts[6][2] = {
				{IMG_CHRS_BODY_ENUMS_OFFSET + rnd(0,IMG_CHRS_BODY_ENUMS_COUNT - 1 - IMG_CHRS_BODY_ENUMS_OFFSET),  rnd(1,33)},

				{ IMG_CHRS_HEAD_ENUMS_OFFSET + rnd(0,IMG_CHRS_HEAD_ENUMS_COUNT - 1 - IMG_CHRS_HEAD_ENUMS_OFFSET), rnd(1,33) },
				{ IMG_CHRS_FACE_ENUMS_OFFSET + rnd(0,IMG_CHRS_FACE_ENUMS_COUNT - 1 - IMG_CHRS_FACE_ENUMS_OFFSET), rnd(1,33) },

				{ IMG_CHRS_HAIR_ENUMS_OFFSET + rnd(0,IMG_CHRS_HAIR_ENUMS_COUNT - 1 - IMG_CHRS_HAIR_ENUMS_OFFSET), rnd(1,33) },
				{ IMG_CHRS_BEARD_ENUMS_OFFSET + rnd(0,IMG_CHRS_BEARD_ENUMS_COUNT - 1 - IMG_CHRS_BEARD_ENUMS_OFFSET),rnd(1,33) },
				{ IMG_CHRS_HELM_ENUMS_OFFSET + rnd(0,IMG_CHRS_HELM_ENUMS_COUNT - 1 - IMG_CHRS_HELM_ENUMS_OFFSET), rnd(1,33) }
		};

		if(rnd(0, 33) > 15)
			char_parts[3][0] = -1;

		if(rnd(0, 33) > 15)
			char_parts[4][0] = -1;

		if(rnd(0, 33) > 15)
			char_parts[5][0] = -1;

		char_set_update(char_id, char_parts);

		{
			RD2SPR_OBJ spr_obj = spr_obj_init(char_id, 0);
			//spr_obj.flags = SPR_FLG_ROT_90 | SPR_FLG_ROT_180;
			spr_obj.replace_colors_id = rnd(0, 32) - 1;
			spr_man_spr_obj_push(&spr_obj);
		}
	}

	//--------------------------------------------------------------------------------------
	// Main game loop
	//--------------------------------------------------------------------------------------
	while(!hal_window_is_close())    // Detect window close button or ESC key
	{
		//----------------------------------------------------------------------------------
		// GUI Update first
		//----------------------------------------------------------------------------------
		gui_update();
		//----------------------------------------------------------------------------------
		// Game Update
		//----------------------------------------------------------------------------------

		if(gui_kb_last_key() == KEY_PRINT_SCREEN)
			hal_save_image(*rd2_scr_get(), "screenshot.png");
		//----------------------------------------------------------------------------------
		// clear screen
		//----------------------------------------------------------------------------------
		rd2_rect_fill(0, 0, game_scr_width(), game_scr_height(), gui_color(PAL_STEELBLUE));
		//rd2_rect(0, 0, game_scr_width(), game_scr_height(), ARGB32(rnd_u8(), rnd_u8(), rnd_u8(), 255));
		//----------------------------------------------------------------------------------				

		//----------------------------------------------------------------------------------
		// GAME SCREEN RENDER
		//----------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------
		spr_man_spr_objs_update();

		{
			RD2SPR_OBJ* spr_obj = spr_man_spr_obj_get(0);
			spr_obj_draw(spr_obj, (p32) { 256, 64 * 0 });
		}
		{
			RD2SPR_OBJ* spr_obj = spr_man_spr_obj_get(1);
			spr_obj_draw(spr_obj, (p32) { 256, 64 * 1 });
		}
		{
			RD2SPR_OBJ* spr_obj = spr_man_spr_obj_get(2);
			spr_obj_draw(spr_obj, (p32) { 256, 64 * 2 });
		}

		if(gui_kb_last_key() == 'R')
		{
			int32 char_parts[6][2] = {
				{IMG_CHRS_BODY_ENUMS_OFFSET + rnd(0,IMG_CHRS_BODY_ENUMS_COUNT - 1 - IMG_CHRS_BODY_ENUMS_OFFSET),  rnd(1,33)},

				{ IMG_CHRS_HEAD_ENUMS_OFFSET + rnd(0,IMG_CHRS_HEAD_ENUMS_COUNT - 1 - IMG_CHRS_HEAD_ENUMS_OFFSET), rnd(1,33) },
				{ IMG_CHRS_FACE_ENUMS_OFFSET + rnd(0,IMG_CHRS_FACE_ENUMS_COUNT - 1 - IMG_CHRS_FACE_ENUMS_OFFSET), rnd(1,33) },

				{ IMG_CHRS_HAIR_ENUMS_OFFSET + rnd(0,IMG_CHRS_HAIR_ENUMS_COUNT - 1 - IMG_CHRS_HAIR_ENUMS_OFFSET), rnd(1,33) },
				{ IMG_CHRS_BEARD_ENUMS_OFFSET + rnd(0,IMG_CHRS_BEARD_ENUMS_COUNT - 1 - IMG_CHRS_BEARD_ENUMS_OFFSET),rnd(1,33) },
				{ IMG_CHRS_HELM_ENUMS_OFFSET + rnd(0,IMG_CHRS_HELM_ENUMS_COUNT - 1 - IMG_CHRS_HELM_ENUMS_OFFSET), rnd(1,33) }
			};

			if(rnd(0, 33) > 15)
				char_parts[3][0] = -1;

			if(rnd(0, 33) > 15)
				char_parts[4][0] = -1;

			if(rnd(0, 33) > 15)
				char_parts[5][0] = -1;

			char_set_update(1, char_parts);
		}

		{
			RD2SPR_OBJ* spr_obj = spr_man_spr_obj_get(3);

			if(gui_kb_last_key() == 'R')
				spr_obj_spr_anim_set(spr_obj, rnd(0, 1));

			spr_obj_draw(spr_obj, (p32) { 256, 64 * 3 });
		}
		//----------------------------------------------------------------------------------
		// GUI DRAW
		//----------------------------------------------------------------------------------
		gui_draw();
		//----------------------------------------------------------------------------------
		char* tst_str = "ÿקלקס ננננ ÎÙØÎÐÐÎ ‗×ÌÞÞ,ÆÄ<>{}[p]ûגאûג Áבב ÛÂÀÛÂ אûגאûג \n1234340890437!\"¹; ,^&*()_+ & & &\nיצףךוםדרשחץתפûגאןנמכהז‎ÿקסלטעüב‏.¸";

		rd2_font_print(game_font_get(GAME_FONT_ID_DOS_RUS), tst_str,
			0, 90, 1, gui_color(PAL_TRANSPARENT), gui_color(PAL_WHITE), FNT_FLG_DRAW);

		rd2_font_print(game_font_get(GAME_FONT_ID_BOLD_RUS), tst_str,
			0, 120, 1, gui_color(PAL_TRANSPARENT), gui_color(PAL_WHITE), FNT_FLG_DRAW);

		rd2_font_print(game_font_get(GAME_FONT_ID_ZX_RUS), tst_str,
			0, 150, 1, gui_color(PAL_TRANSPARENT), gui_color(PAL_WHITE), FNT_FLG_DRAW);

		rd2_font_print(game_font_get(GAME_FONT_ID_MICROFONT), tst_str,
			0, 180, 1, gui_color(PAL_TRANSPARENT), gui_color(PAL_WHITE), FNT_FLG_DRAW);
		//----------------------------------------------------------------------------------
		// Draw
		//----------------------------------------------------------------------------------
		hal_draw_begin();

		hal_cls(ARGB32(0, 0, 255, 255));
		//hal_cls(ARGB32(0, 0, 0, 255));

		game_scr_draw();

		hal_draw_fps(20, 0);
		hal_draw_end();
		//----------------------------------------------------------------------------------
	}

	//--------------------------------------------------------------------------------------
	// De-Initialization
	//--------------------------------------------------------------------------------------
	hal_system_shutdown();
	game_shutdown();
	return 0;
}
