void rd2_spr_blit(RD2IMG* src, rd2i dx, rd2i dy, rd2i sx, rd2i sy, rd2i w, rd2i h)
{
	int cw = rd2scr.width;
	int ch = rd2scr.height;

	CLIP();

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		for(int j = 0; j < h; j++)
		{
			rd2c* ts = &src->pixels32[(sy + j) * src->width + sx];
			for(int k = 0; k < w; k++)
				rd2ps.plot(dx + k, dy + j, *(ts + k));
		}
	}
	else
	{
		rd2c* ts = &src->pixels32[sy * src->width + sx];
		rd2c* td = &rd2scr.pixels32[dy * rd2scr.width + dx];
		int st = src->width;
		int dt = rd2scr.width;

		do
		{
			//memcpy(td, ts, w * sizeof(rd2c));

			for(int x = 0; x < w; x++)
				rd2ps.blend(td + x, *(ts + x)); //td[x] = ts[x];

			ts += st;
			td += dt;
		}
		while(--h);
	}
}

void rd2_hline_texture(RD2IMG* src, rd2i x0, rd2i x1, rd2i y, rd2i tx0, rd2i ty0, float duvx, float duvy)
{
	rd2i h;
	if(y < 0 || y >= rd2scr.height) return;
	if(x0 > x1) SWAP(x0, x1, h);
	if(x0 < 0) x0 = 0;
	if(x1 >= rd2scr.width) x1 = rd2scr.width - 1;
	if(x0 > x1) return;

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		int32 ty = y - ty0;
		int32 tx = x0 - tx0;
		float tu = duvx * tx + duvy * ty;
		float tv = -duvy * tx + duvx * ty;

		h = x1 - x0;
		do
		{
			int32 tui = tu;
			int32 tvi = tv;

			if(tui < 0)
				tui = 0;

			if(tvi < 0)
				tvi = 0;

			if(tui >= src->width)
				tui = src->width - 1;

			if(tvi >= src->height)
				tvi = src->height - 1;

			uint32 c = *(src->pixels32 + tvi * src->width + tui);
			rd2ps.plot(x0, y, c);
			x0++;
			tu += duvx;
			tv -= duvy;
		}
		while(h--);
	}
	else
	{
		int32 tx = x0 - tx0;
		int32 ty = y - ty0;
		float tu = duvx * tx + duvy * ty;
		float tv = -duvy * tx + duvx * ty;

		rd2c* dst = rd2scr.pixels32 + y * rd2scr.width + x0;
		h = x1 - x0;
		do
		{
			int32 tui = tu;
			int32 tvi = tv;

			if(tui < 0)
				tui = 0;

			if(tvi < 0)
				tvi = 0;

			if(tui >= src->width)
				tui = src->width - 1;

			if(tvi >= src->height)
				tvi = src->height - 1;

			uint32 c = *(src->pixels32 + tvi * src->width + tui);
			rd2ps.blend(dst++, c); //*dst++ = c;
			x0++;
			tu += duvx;
			tv -= duvy;
		}
		while(h--);
	}
}

void rd2_spr_quad(RD2IMG* src, rd2i* vxy, int vcount)
{
	int32 x_min = INT32_MAX;
	int32 x_max = INT32_MIN;
	int32 y_min = INT32_MAX;
	int32 y_max = INT32_MIN;

	for(int i = 0; i < vcount; i++)
	{
		rd2i x0, y0;

		x0 = vxy[i * 2];
		y0 = vxy[i * 2 + 1];

		x_min = MIN(x_min, x0);
		x_max = MAX(x_max, x0);
		y_min = MIN(y_min, y0);
		y_max = MAX(y_max, y0);
	}

	if(x_min < 0) x_min = 0;
	if(x_max >= rd2scr.width) x_max = rd2scr.width - 1;
	if(x_min > x_max)
		return;

	if(y_min < 0) y_min = 0;
	if(y_max >= rd2scr.height) y_max = rd2scr.height - 1;
	if(y_min > y_max)
		return;

	for(int i = y_min; i <= y_max; i++)
	{
		tx0x1[i][0] = INT32_MAX;
		tx0x1[i][1] = INT32_MIN;
	}

	for(int i = 0; i < vcount; i++)
	{
		rd2i x0, y0, x1, y1;

		x0 = vxy[i * 2];
		y0 = vxy[i * 2 + 1];

		int i1 = (i + 1) % vcount;

		x1 = vxy[i1 * 2];
		y1 = vxy[i1 * 2 + 1];

		rd2_line_tx0x1(x0, y0, x1, y1);
	}

	int32 tx0 = vxy[0];
	int32 ty0 = vxy[1];

	float dux = (vxy[1 * 2] - vxy[0]);
	float duy = (vxy[1 * 2 + 1] - vxy[1]);

	float du_len_sqr = dux * dux + duy * duy;

	float w = MIN(src->width, src->height);
	float duvx = dux * w / du_len_sqr;
	float duvy = duy * w / du_len_sqr;

	for(int y = y_min; y <= y_max; y++)
		rd2_hline_texture(src, tx0x1[y][0], tx0x1[y][1], y, tx0, ty0, duvx, duvy);
}

void rd2_hline_texture_fast(RD2IMG* src, rd2i x0, rd2i x1, rd2i y, rd2i tx0, rd2i ty0, fp32 duvx)
{
	rd2i h;
	if(y < 0 || y >= rd2scr.height) return;
	if(x0 > x1) SWAP(x0, x1, h);
	if(x0 < 0) x0 = 0;
	if(x1 >= rd2scr.width) x1 = rd2scr.width - 1;
	if(x0 > x1) return;

	int32 tx = x0 - tx0;
	int32 ty = y - ty0;

	//float tu = duvx * tx;
	fp32 tu = FP10_MUL_INT(duvx, tx);

	//float tv = duvx * ty;
	fp32 tv = FP10_MUL_INT(duvx, ty);

	int32 tvi = FP10_TO_INT(tv);

	if(tvi < 0)
		tvi = 0;

	if(tvi >= src->height)
		tvi = src->height - 1;

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		rd2c* tsrc = src->pixels32 + tvi * src->width;

		h = x1 - x0;
		do
		{
			int32 tui = FP10_TO_INT(tu);

			if(tui < 0)
				tui = 0;

			if(tui >= src->width)
				tui = src->width - 1;

			uint32 c = *(tsrc + tui);
			rd2ps.plot(x0, y, *(tsrc + tui));
			//*tdst++ = c;
			x0++;
			tu += duvx;
		}
		while(h--);
	}
	else
	{
		rd2c* tdst = rd2scr.pixels32 + y * rd2scr.width + x0;
		rd2c* tsrc = src->pixels32 + tvi * src->width;

		h = x1 - x0;
		do
		{
			int32 tui = FP10_TO_INT(tu);

			if(tui < 0)
				tui = 0;

			if(tui >= src->width)
				tui = src->width - 1;

			uint32 c = *(tsrc + tui);
			rd2ps.blend(tdst++, c);
			//*tdst++ = c;
			//x0++;
			tu += duvx;
		}
		while(h--);
	}
}

void rd2_spr_quad_fast(RD2IMG* src, rd2i* vxy, int vcount)
{
	int32 x_min = INT32_MAX;
	int32 x_max = INT32_MIN;
	int32 y_min = INT32_MAX;
	int32 y_max = INT32_MIN;

	for(int i = 0; i < vcount; i++)
	{
		rd2i x0, y0;

		x0 = vxy[i * 2];
		y0 = vxy[i * 2 + 1];

		x_min = MIN(x_min, x0);
		x_max = MAX(x_max, x0);
		y_min = MIN(y_min, y0);
		y_max = MAX(y_max, y0);
	}

	if(x_min < 0) x_min = 0;
	if(x_max >= rd2scr.width) x_max = rd2scr.width - 1;
	if(x_min > x_max)
		return;

	if(y_min < 0) y_min = 0;
	if(y_max >= rd2scr.height) y_max = rd2scr.height - 1;
	if(y_min > y_max)
		return;

	for(int i = y_min; i <= y_max; i++)
	{
		tx0x1[i][0] = INT32_MAX;
		tx0x1[i][1] = INT32_MIN;
	}

	for(int i = 0; i < vcount; i++)
	{
		rd2i x0, y0, x1, y1;

		x0 = vxy[i * 2];
		y0 = vxy[i * 2 + 1];

		int i1 = (i + 1) % vcount;

		x1 = vxy[i1 * 2];
		y1 = vxy[i1 * 2 + 1];

		rd2_line_tx0x1(x0, y0, x1, y1);
	}

	int32 tx0 = vxy[0];
	int32 ty0 = vxy[1];

	//float dux = (vxy[1 * 2] - vxy[0]);
	int32 dux = (vxy[1 * 2] - vxy[0]);

	//float du_len_sqr = dux * dux;
	int32 du_len_sqr = (dux * dux);

	if(du_len_sqr == 0)
		return;

	int32 w = MIN(src->width, src->height);

	//float duvx = dux * w / du_len_sqr;
	fp32 duvx = FP10_INT(dux * w);
	duvx = FP10_DIV_INT(duvx, du_len_sqr);

	for(int y = y_min; y <= y_max; y++)
		rd2_hline_texture_fast(src, tx0x1[y][0], tx0x1[y][1], y, tx0, ty0, duvx);
}

void rd2_spr_rc(RD2IMG* src, rd2i x, rd2i y, float scale, float radians) // support scale and rotation
{
	rd2_spr_r(src, x, y, scale, src->width / 2, src->height / 2, radians);
}

void rd2_spr_r(RD2IMG* src, rd2i x, rd2i y, float scale, rd2i rc_x, rd2i rc_y, float radians) // support scale and rotation
{
	static int32 vxyi[4 * 2];
	static d2f vxy[4 * 2];

	rc_x *= scale;
	rc_y *= scale;

	float sw = src->width;
	float sh = src->height;
	sw *= scale;
	sh *= scale;
	sw -= 1;
	sh -= 1;

	vxy[0 * 2 + 0] = 0;
	vxy[0 * 2 + 1] = 0;

	vxy[1 * 2 + 0] = sw;
	vxy[1 * 2 + 1] = 0;

	vxy[2 * 2 + 0] = sw;
	vxy[2 * 2 + 1] = sh;

	vxy[3 * 2 + 0] = 0;
	vxy[3 * 2 + 1] = sh;

	d2t t2 = d2t_set(d2v_set(x + rc_x, y + rc_y), radians);

	for(int i = 0; i < 4; i++)
	{
		d2v v2 = d2v_set(vxy[i * 2 + 0] - rc_x, vxy[i * 2 + 1] - rc_y);
		v2 = d2t_mul_v(t2, v2);

		vxyi[i * 2 + 0] = d2_round(v2.x); // round to int!
		vxyi[i * 2 + 1] = d2_round(v2.y); // round to int!
	}

	rd2_spr_quad(src, vxyi, 4);
}

void rd2_spr(RD2IMG* src, rd2i x, rd2i y)
{
	rd2_spr_blit(src, x, y, 0, 0, src->width, src->height);
}

void rd2_spr_fast(RD2IMG* src, rd2i x, rd2i y, float scale) // support only scale no rotation
{
	static int32 vxyi[4 * 2];
	static d2f vxy[4 * 2];

	if(scale == 1.0f)
	{
		rd2_spr_blit(src, x, y, 0, 0, src->width, src->height);
		return;
	}

	float sw = src->width;
	float sh = src->height;
	sw *= scale;
	sh *= scale;
	sw -= 1;
	sh -= 1;

	vxy[0 * 2 + 0] = 0;
	vxy[0 * 2 + 1] = 0;

	vxy[1 * 2 + 0] = sw;
	vxy[1 * 2 + 1] = 0;

	vxy[2 * 2 + 0] = sw;
	vxy[2 * 2 + 1] = sh;

	vxy[3 * 2 + 0] = 0;
	vxy[3 * 2 + 1] = sh;

	for(int i = 0; i < 4; i++)
	{
		vxyi[i * 2 + 0] = d2_round(vxy[i * 2 + 0] + x); // round to int!
		vxyi[i * 2 + 1] = d2_round(vxy[i * 2 + 1] + y); // round to int!
	}

	rd2_spr_quad_fast(src, vxyi, 4);
}
