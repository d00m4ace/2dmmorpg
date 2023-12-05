//--------------------------------------------------------------------------------------
int32 rd2_font_draw_str(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale, rd2c paper, rd2c ink)
{
	const uint8* cstr = txt;
	rd2i cx = x;
	rd2i cy = y;

	while(*cstr)
	{
		rd2c origin_colors[] = { ARGB32(255, 255, 255, 255) };
		rd2c replace_colors[] = { ink };

		int32 chr_w;

		if(*cstr == ' ')
			chr_w = fnt->space_width;
		else
			chr_w = fnt->cw[*cstr];

		if(paper != 0 || ink != 0)
		{
			rd2_img_fill(&fnt->citmp[1], 0, 0, fnt->citmp[0].width, fnt->citmp[0].height, 0);
			rd2_img_blit(&fnt->citmp[1], &fnt->ci[*cstr], 0, 0, 0, 0, fnt->citmp[1].width, fnt->citmp[1].height);
			rd2_img_color_replace(&fnt->citmp[1], origin_colors, replace_colors, 1);

			rd2_img_fill(&fnt->citmp[0], 0, 0, chr_w, fnt->citmp[0].height, paper);
			if(fnt->citmp[0].width - chr_w > 0)
				rd2_img_fill(&fnt->citmp, chr_w, 0, fnt->citmp[0].width - chr_w, fnt->citmp[0].height, 0);
			rd2_img_blit(&fnt->citmp[0], &fnt->citmp[1], 0, 0, 0, 0, fnt->citmp[0].width, fnt->citmp[0].height);

			rd2_spr_fast(&fnt->citmp[0], cx, cy, scale);
		}

		cx += chr_w * scale;

		if(fnt->padding_width > 0)
			rd2_rect_fill(cx, cy, fnt->padding_width, fnt->char_height * scale, paper);

		cx += fnt->padding_width;

		cstr++;
	}

	return cx - x;
}

int32 rd2_font_draw_str_calc_width(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale)
{
	const uint8* cstr = txt;
	rd2i cx = x;
	rd2i cy = y;

	while(*cstr)
	{
		int32 chr_w;

		if(*cstr == ' ')
			chr_w = fnt->space_width;
		else
			chr_w = fnt->cw[*cstr];

		cx += chr_w * scale + fnt->padding_width;
		cstr++;
	}

	return cx - x;
}

point32 rd2_font_print(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale, rd2c paper, rd2c ink, uint32 fnt_flgs)
{
	int32 calc_width = 0;

	rd2i cx = x;
	rd2i cy = y;

	VECM* vecm = mem_get_vecm(MEM_VECM_ID_FONT_TEXT);
	vecm_clear(vecm);
	mem_gen_tokens(vecm, txt);

	int32 tcount = vecm_count(vecm);

	bool is_flash_color = false;
	rd2c cur_paper = paper;
	rd2c cur_ink = ink;

	int32 chr_h = fnt->char_height * scale;

	int32 color_stack_count = 0;
	int32 arg_str_count = 0;

	int32 arg_digit = 0;
	int32 repeat_count = 1;

	VECM* varg = mem_get_vecm(MEM_VECM_ID_FONT_TEXT_ARG);

	char chr[2] = { 0 };
	const char* tstr = NULL;

	for(int i = 0; i < tcount; i++)
	{
		if(vecm_elm_is_word(vecm, i))
		{
			tstr = vecm_elm(vecm, i);
		}
		else if(vecm_elm_equal_chr(vecm, i, '#'))
		{
			if(vecm_elm_equal_chr(vecm, i + 1, '#'))
			{
				chr[0] = '#';
				tstr = &chr;
				i++;
			}
			else if(vecm_elm_equal_chr(vecm, i + 1, '!'))
			{
				is_flash_color = !is_flash_color;
				i++;
			}
			else if(vecm_elm_is_digit(vecm, i + 1))
			{
				const char* tdgt = vecm_elm(vecm, i + 1);
				arg_digit = txt_str_to_int(tdgt, NULL);
				i++;

				if(vecm_elm_equal_chr(vecm, i + 1, '*'))
				{
					repeat_count = arg_digit;
					arg_digit = 0;
					i++;
				}
				else if(vecm_elm_equal_chr(vecm, i + 1, '$'))
				{
					if(arg_digit >= 0)
						cur_paper = gui_color(arg_digit % PALETTE_COLORS_ENUM_COUNT);
					else
						cur_paper = paper;

					arg_digit = 0;
					i++;
				}
				else if(vecm_elm_equal_chr(vecm, i + 1, '@'))
				{
					if(arg_digit >= 0)
						cur_ink = gui_color(arg_digit % PALETTE_COLORS_ENUM_COUNT);
					else
						cur_ink = ink;

					arg_digit = 0;
					i++;
				}
				else if(vecm_elm_equal_chr(vecm, i + 1, '&'))
				{
					while(arg_digit >= 0 && repeat_count > 0)
					{
						RD2IMG* img = game_imgs_get((arg_digit) % GAME_IMGS_MAX_COUNT);
						//VERTICAL_ALIGN_ENUM algn = VALIGN_TOP;
						VERTICAL_ALIGN_ENUM algn = VALIGN_MIDDLE;
						//VERTICAL_ALIGN_ENUM algn = VALIGN_BOTTOM;

						rd2c c_p = cur_paper;
						rd2c c_i = cur_ink;

						int32 calc_w = img->width * scale;

						if((fnt_flgs & FNT_FLG_WORD_WRAP) && (cx + calc_w) > rd2_scr_get()->width)
						{
							cx = x;

							if((cy + chr_h * 2) > rd2_scr_get()->height)
								break;

							cy += chr_h + fnt->padding_height;
						}

						if(fnt_flgs & FNT_FLG_DRAW)
						{
							int32 valgn = gui_style_calc_valign(algn, fnt->char_height * scale, img->height * scale);

							rd2_rect_fill(cx, cy + valgn, calc_w, img->height * scale, c_p);

							if(!(is_flash_color && hal_is_draw_flash_time()))
								rd2_spr_fast(img, cx, cy + valgn, scale);

							cx += calc_w;

							if(fnt->padding_width > 0)
								rd2_rect_fill(cx, cy + valgn, fnt->padding_width, img->height * scale, c_p);
						}
						else
						{
							cx += calc_w;
						}

						cx += fnt->padding_width;

						calc_width = MAX(calc_width, cx - x);

						repeat_count--;
					}

					tstr = NULL;
					arg_digit = 0;
					repeat_count = 1;
					i++;
				}
			}
		}
		else
		{
			tstr = vecm_elm(vecm, i);
		}

		if(tstr != NULL)
		{
			while(repeat_count > 0)
			{
				rd2c c_p = cur_paper;
				rd2c c_i = cur_ink;

				if(is_flash_color && hal_is_draw_flash_time())
				{
					c_i = cur_paper;
					c_p = cur_ink;

					if(!(cur_paper & 0xF8000000))
						c_p = c_i = 0;
				}

				{
					int32 calc_w = rd2_font_draw_str_calc_width(fnt, tstr, cx, cy, scale);

					if((*tstr == '\n') || (fnt_flgs & FNT_FLG_WORD_WRAP) && (cx + calc_w > rd2_scr_get()->width))
					{
						cx = x;

						if((cy + chr_h * 2) > rd2_scr_get()->height)
							break;

						cy += chr_h + fnt->padding_height;
					}

					if(*tstr != '\n')
					{
						if(fnt_flgs & FNT_FLG_DRAW)
							rd2_font_draw_str(fnt, tstr, cx, cy, scale, c_p, c_i);

						cx += calc_w;
					}
				}

				calc_width = MAX(calc_width, cx - x);

				repeat_count--;
			}

			tstr = NULL;
			arg_digit = 0;
			repeat_count = 1;
		}
	}

	return (point32) { calc_width, (cy - y) + chr_h };
}
//--------------------------------------------------------------------------------------