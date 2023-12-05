//--------------------------------------------------------------------------------------
RD2FONT rd2_font_init(int32 width, int32 height)
{
	RD2FONT font;

	char* char_bitmap_heap = CALLOC(1, width * height * sizeof(rd2c) * RD2FONT_MAX_CHAR);
	ASSERT(char_bitmap_heap != NULL);
	font.data = char_bitmap_heap;

	for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
	{
		font.ci[i].pixels32 = char_bitmap_heap + i * (width * height * sizeof(rd2c));
		font.ci[i].width = width;
		font.ci[i].height = height;
		font.cw[i] = width;
	}

	font.char_width = width;
	font.char_height = height;
	font.space_width = width;

	font.padding_width = font.padding_height = 0;

	font.citmp[0] = rd2_img_init(width, height);
	font.citmp[1] = rd2_img_init(width, height);

	return font;
}

void rd2_font_free(RD2FONT* fnt)
{
	CFREE(fnt->data);
	fnt->data = NULL;

	rd2_img_free(&fnt->citmp[0]);
	rd2_img_free(&fnt->citmp[1]);
}

void rd2_font8_set(RD2FONT* dst, char* data, int char_count)
{
	for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
	{
		uint8* cs = data + i * 8;
		rd2c* cd = dst->ci[i].pixels32;

		for(int y = 0; y < 8; y++)
		{
			for(int x = 0; x < 8; x++)
				if(*cs & (1 << (7 - x)))
					cd[x] = ARGB32(255, 255, 255, 255);
				else
					cd[x] = ARGB32(0, 0, 0, 0);
			cs++;
			cd += 8;
		}
	}
}

bool rd2_font_save_image(RD2FONT* fnt, const char* file_name)
{
	int cw = 9;
	int ch = 9;
	RD2IMG if8 = rd2_img_init(16 * cw, 16 * ch);
	rd2_img_fill(&if8, 0, 0, if8.width, if8.height, ARGB32(0, 0, 0, 0));

	{
		for(int i = 0; i < 16; i++)
		{
			rd2_img_fill(&if8, 0, i * (ch), if8.width, 1, ARGB32(0, 0, 0, 255));
			rd2_img_fill(&if8, i * (cw), 0, 1, if8.height, ARGB32(0, 0, 0, 255));
		}
	}

	{
		for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
		{
			int x = i % 16;
			int y = (i >> 4) % 16;
			rd2_img_blit(&if8, &fnt->ci[i], 1 + cw * x, 1 + ch * y, 0, 0, fnt->ci[i].width, fnt->ci[i].height);
		}
	}

	return hal_save_image(if8, file_name);
}

bool rd2_font_load_image(RD2FONT* fnt, const char* file_name)
{
	int cw = fnt->char_width + 1;
	int ch = fnt->char_height + 1;

	RD2IMG if8 = hal_load_image(file_name);

	{
		for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
		{
			int x = i % 16;
			int y = (i >> 4) % 16;
			rd2_img_blit(&fnt->ci[i], &if8, 0, 0, 1 + cw * x, 1 + ch * y, fnt->ci[i].width, fnt->ci[i].height);
		}
	}

	return true;
}

void rd2_font_trim_chars(RD2FONT* fnt)
{
	for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
	{
		RD2IMG* ci = &fnt->ci[i];

		int x = 0;
		int xpass = 0;

		while(xpass < ci->width)
		{
			if(x == 0 && rd2_img_is_vblank(ci, x))
				rd2_img_vcut(ci, x);
			else if(x < (ci->width - 1) && rd2_img_is_vblank(ci, x) && rd2_img_is_vblank(ci, x + 1))
				rd2_img_vcut(ci, x);
			else
				x++;

			xpass++;
		}

		x = ci->width - 1;

		while(x)
		{
			if(!rd2_img_is_vblank(ci, x))
				break;
			x--;
		}

		fnt->cw[i] = x + 1;
	}
}

void rd2_font_thin_chars(RD2FONT* fnt, int vequal_max)
{
	if(vequal_max == -1)
		vequal_max = 1;

	for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
	{
		RD2IMG* ci = &fnt->ci[i];

		int x = 0;
		int xpass = 0;
		int vequal = 0;

		while(xpass < ci->width)
		{
			if(x == 0 && rd2_img_is_vblank(ci, x))
				rd2_img_vcut(ci, x);
			else if(x < (ci->width - 1) && rd2_img_is_vblank(ci, x) && rd2_img_is_vblank(ci, x + 1))
				rd2_img_vcut(ci, x);
			else if(vequal < vequal_max && x < (ci->width - 1) && rd2_img_is_vequal(ci, x, x + 1))
			{
				rd2_img_vcut(ci, x);
				vequal++;
			}
			else
				x++;

			xpass++;
		}

		x = ci->width - 1;

		while(x)
		{
			if(!rd2_img_is_vblank(ci, x))
				break;
			x--;
		}

		fnt->cw[i] = x + 1;
	}
}

RD2FONT rd2_font_border(RD2FONT* fnt, int border, rd2c color_border)
{
	RD2FONT bfnt = rd2_font_init(fnt->char_width + border * 2, fnt->char_height + border * 2);

	for(int i = 0; i < RD2FONT_MAX_CHAR; i++)
	{
		RD2IMG* ci = &fnt->ci[i];
		RD2IMG* di = &bfnt.ci[i];

		rd2_img_fill(di, 0, 0, di->width, di->height, ARGB32(0, 0, 0, 0));

		for(int b = border; b > 0; b--)
		{
			rd2_img_blit(di, ci, border - b, border, 0, 0, ci->width, ci->height);
			rd2_img_blit(di, ci, border, border - b, 0, 0, ci->width, ci->height);
			rd2_img_blit(di, ci, border + b, border, 0, 0, ci->width, ci->height);
			rd2_img_blit(di, ci, border, border + b, 0, 0, ci->width, ci->height);

			//rd2_img_blit(di, ci, border - b, border - b, 0, 0, ci->width, ci->height);
			//rd2_img_blit(di, ci, border + b, border + b, 0, 0, ci->width, ci->height);
		}

		rd2_img_color_replace(di, (rd2c[]) { ARGB32(255, 255, 255, 255) }, (rd2c[]) { color_border }, 1);

		rd2_img_blit(di, ci, border, border, 0, 0, ci->width, ci->height);

		bfnt.cw[i] = fnt->char_width;
	}

	bfnt.space_width = fnt->space_width;
	bfnt.padding_width = fnt->padding_width;
	bfnt.padding_height = fnt->padding_height;
	return bfnt;
}
//--------------------------------------------------------------------------------------