//--------------------------------------------------------------------------------------
RD2IMG rd2_img_init(int32 width, int32 height)
{
	RD2IMG img;
	img.width = width;
	img.height = height;
	img.pixels32 = CALLOC(1, width * height * sizeof(rd2c));
	return img;
}

void rd2_img_free(RD2IMG* img)
{
	CFREE(img->pixels32);
	img->pixels32 = NULL;
	img->width = img->height = 0;
}
//--------------------------------------------------------------------------------------
void rd2_img_raw_copy(RD2IMG* img, void* raw_data, int32 raw_bytes_per_line, int32 raw_width, int32 raw_height)
{
	for(int y = 0; y < img->height; y++)
		for(int x = 0; x < img->width; x++)
			img->pixels32[y * img->width + x] = *((uint32*)((uint8*)raw_data + (y)*raw_bytes_per_line) + x);
}

void rd2_raw_img_copy(RD2IMG* img, void* raw_data, int32 raw_bytes_per_line, int32 raw_width, int32 raw_height)
{
	for(int y = 0; y < img->height; y++)
		for(int x = 0; x < img->width; x++)
			*((uint32*)((uint8*)raw_data + (y)*raw_bytes_per_line) + x) = img->pixels32[y * img->width + x];
}
//--------------------------------------------------------------------------------------
void rd2_img_color_replace(RD2IMG* dst, rd2c* origin_colors, rd2c* replace_colors, int colors)
{
	int w = dst->width;
	int h = dst->height;

	rd2c* td = dst->pixels32;
	do
	{
		for(int x = 0; x < w; x++)
		{
			if(td[x] & 0xF8000000)
			{
				for(int c = 0; c < colors; c++)
				{
					if(td[x] == origin_colors[c])
					{
						td[x] = replace_colors[c];
						break;
					}
				}
			}
		}

		td += dst->width;
	}
	while(--h);
}

void rd2_img_color_match_replace(RD2IMG* dst, rd2c* match_colors, int colors)
{
	int w = dst->width;
	int h = dst->height;

	rd2c* td = dst->pixels32;
	do
	{
		for(int x = 0; x < w; x++)
		{
			if(td[x] & 0xF8000000)
			{
				int32 src_ca = ARGB32_GETA(td[x]);
				int32 src_cr = ARGB32_GETR(td[x]);
				int32 src_cb = ARGB32_GETB(td[x]);
				int32 src_cg = ARGB32_GETG(td[x]);

				rd2c match_col = 0;
				int32 match_col_diff = 255 * 255 * 4;

				for(int c = 0; c < colors; c++)
				{
					int32 dst_ca = ARGB32_GETA(match_colors[c]);
					int32 dst_cr = ARGB32_GETR(match_colors[c]);
					int32 dst_cb = ARGB32_GETB(match_colors[c]);
					int32 dst_cg = ARGB32_GETG(match_colors[c]);

					int32 cur_match_col_diff =
						(src_ca - dst_ca) * (src_ca - dst_ca) +
						(src_cr - dst_cr) * (src_cr - dst_cr) +
						(src_cb - dst_cb) * (src_cb - dst_cb) +
						(src_cg - dst_cg) * (src_cg - dst_cg);

					if(cur_match_col_diff < match_col_diff)
					{
						match_col = match_colors[c];
						match_col_diff = cur_match_col_diff;
					}
				}

				if(match_col_diff != 0xFFFFFFFF)
					td[x] = match_col;// 0xFF0000FF;// match_col;
			}
		}

		td += dst->width;
	}
	while(--h);
}

void rd2_img_copy(RD2IMG* dst, RD2IMG* src)
{
	ASSERT(dst->width == src->width && dst->height == src->height);

	int w = dst->width;
	int h = dst->height;

	rd2c* ts = src->pixels32;
	rd2c* td = dst->pixels32;
	do
	{
		memcpy(td, ts, w * sizeof(rd2c));
		ts += dst->width;
		td += dst->width;
	}
	while(--h);
}

void rd2_img_blit(RD2IMG* dst, RD2IMG* src, rd2i dx, rd2i dy, rd2i sx, rd2i sy, rd2i w, rd2i h)
{
	int cw = dst->width;
	int ch = dst->height;

	CLIP();

	rd2c* ts = &src->pixels32[sy * src->width + sx];
	rd2c* td = &dst->pixels32[dy * dst->width + dx];
	int st = src->width;
	int dt = dst->width;
	do
	{
		for(int x = 0; x < w; x++)
			if(ts[x] & 0xF8000000)
				td[x] = ts[x];
		ts += st;
		td += dt;
	}
	while(--h);
}

void rd2_img_fill(RD2IMG* dst, rd2i x, rd2i y, rd2i w, rd2i h, rd2c color)
{
	int32 dt, i;

	if(x < 0)
	{
		w += x;
		x = 0;
	}
	if(y < 0)
	{
		h += y;
		y = 0;
	}

	if(x + w > dst->width)
		w = dst->width - x;
	if(y + h > dst->height)
		h = dst->height - y;

	if(w <= 0 || h <= 0)
		return;

	rd2c* dstp = dst->pixels32 + y * dst->width + x;
	dt = dst->width;

	do
	{
		for(i = 0; i < w; i++)
			dstp[i] = color;
		dstp += dt;
	}
	while(--h);
}

void rd2_img_hflip(RD2IMG* src)
{
	rd2c* cs, * cd, ct;
	for(int32 y = 0; y < src->height / 2; y++)
		for(int32 x = 0; x < src->width; x++)
		{
			cs = &src->pixels32[y * src->width + x];
			cd = &src->pixels32[(src->height - 1 - y) * src->width + x];
			SWAP(*cs, *cd, ct);
		}
}

void rd2_img_vflip(RD2IMG* src)
{
	rd2c* cs, * cd, ct;
	for(int32 y = 0; y < src->height; y++)
		for(int32 x = 0; x < src->width / 2; x++)
		{
			cs = src->pixels32 + y * src->width;
			cd = cs + src->width - 1 - x; cs += x;
			SWAP(*cs, *cd, ct);
		}
}

void rd2_img_rotate90(RD2IMG* dst, RD2IMG* src)
{
	ASSERT(dst->width * dst->height == src->width * src->height);

	dst->width = src->height;
	dst->height = src->width;
	rd2c* cs, * cd;

	for(int32 y = 0; y < src->height; y++)
		for(int32 x = 0; x < src->width; x++)
		{
			cs = &src->pixels32[y * src->width + x];
			cd = &dst->pixels32[x * dst->width + dst->width - 1 - y];
			*cd = *cs;
		}
}

bool rd2_img_is_vblank(RD2IMG* src, rd2i x)
{
	rd2c* cs;

	ASSERT(x >= 0 && x < src->width);

	for(int32 y = 0; y < src->height; y++)
	{
		cs = &src->pixels32[y * src->width + x];
		if(*cs & 0xF8000000)
			return false;
	}

	return true;
}

bool rd2_img_is_vequal(RD2IMG* src, rd2i x0, rd2i x1)
{
	rd2c* cs0, * cs1;

	ASSERT(x0 >= 0 && x0 < src->width);
	ASSERT(x1 >= 0 && x1 < src->width);

	for(int32 y = 0; y < src->height; y++)
	{
		cs0 = src->pixels32 + y * src->width;
		cs1 = cs0 + x1;
		cs0 += x0;
		if(*cs0 != *cs1)
			return false;
	}

	return true;
}

void rd2_img_vcut(RD2IMG* src, rd2i x0)
{
	rd2c* cs0;

	ASSERT(x0 >= 0 && x0 < src->width);

	for(int32 y = 0; y < src->height; y++)
	{
		cs0 = src->pixels32 + y * src->width;
		for(int32 x = x0; x < src->width; x++)
		{
			if(x == src->width - 1)
				*(cs0 + x) = 0;
			else
				*(cs0 + x) = *(cs0 + x + 1);
		}
	}
}

void rd2_img_y_shift(RD2IMG* dst, RD2IMG* src, int32 y_offs, int32 y_shift)
{
	rd2_img_fill(dst, 0, 0, dst->width, dst->height, ARGB32(0, 0, 0, 0));

	rd2_img_blit(dst, src, 0, y_offs, 0, 0, dst->width, dst->height - y_shift);
	rd2_img_blit(dst, src, 0, dst->height - y_shift, 0, dst->height - y_shift, dst->width, y_shift);
}
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
#define RD2_IMG_STACK_MAX_SIZE 8
#define RD2_IMG_STACK_MAX_IMG_SIZE 256
static RD2IMG rd2_img_stack[RD2_IMG_STACK_MAX_SIZE] = { 0 };
static int32 rd2_img_stack_count = 0;

void rd2_img_stack_init(void)
{
	rd2_img_stack_count = 0;

	for(int i = 0; i < RD2_IMG_STACK_MAX_SIZE; i++)
	{
		rd2_img_free(&rd2_img_stack[i]);
		rd2_img_stack[i] = rd2_img_init(RD2_IMG_STACK_MAX_IMG_SIZE, RD2_IMG_STACK_MAX_IMG_SIZE);
	}
}

RD2IMG rd2_img_stack_tmp(int32 width, int32 height, rd2c color)
{
	ASSERT(width <= RD2_IMG_STACK_MAX_IMG_SIZE);
	ASSERT(height <= RD2_IMG_STACK_MAX_IMG_SIZE);

	RD2IMG img_tmp = rd2_img_stack[rd2_img_stack_count];

	img_tmp.width = width;
	img_tmp.height = height;

	rd2_img_fill(&img_tmp, 0, 0, width, height, color);

	rd2_img_stack_count++;

	if(rd2_img_stack_count >= RD2_IMG_STACK_MAX_SIZE)
		rd2_img_stack_count = 0;

	return img_tmp;
}
//--------------------------------------------------------------------------------------