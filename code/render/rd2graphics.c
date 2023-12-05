//--------------------------------------------------------------------------------------
#define MAX_SCR_HEIGHT (1024)
static rd2i tx0x1[MAX_SCR_HEIGHT][2];
//--------------------------------------------------------------------------------------
void rd2_cls(rd2c color)
{
	rd2_img_fill(&rd2scr, 0, 0, rd2scr.width, rd2scr.height, color);
}

void rd2_plot(rd2i x, rd2i y, rd2c color)
{
	if(x >= 0 && x < rd2scr.width && y >= 0 && y < rd2scr.height)
	{
		rd2c* dst = rd2scr.pixels32 + y * rd2scr.width + x;
		rd2ps.blend(dst, color);
	}
}

void rd2_rect_fill(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color)
{
	int32 dt, i;

	//if(w <= 0 || h <= 0)
		//return;

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

	if(x + w > rd2scr.width)
		w = rd2scr.width - x;
	if(y + h > rd2scr.height)
		h = rd2scr.height - y;

	if(w <= 0 || h <= 0)
		return;

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		w += x;	h += y;
		for(int j = y; j < h; j++)
			for(int k = x; k < w; k++)
				rd2ps.plot(k, j, color);
	}
	else
	{
		rd2c* dst = rd2scr.pixels32 + y * rd2scr.width + x;
		dt = rd2scr.width;

		do
		{
			for(i = 0; i < w; i++)
				rd2ps.blend(dst + i, color);
			dst += dt;
		}
		while(--h);
	}
}

void rd2_hline(rd2i x0, rd2i x1, rd2i y, rd2c color)
{
	rd2i h;
	if(y < 0 || y >= rd2scr.height) return;
	if(x0 > x1) SWAP(x0, x1, h);
	if(x0 < 0) x0 = 0;
	if(x1 >= rd2scr.width)
		x1 = rd2scr.width - 1;
	if(x0 > x1)
		return;

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		for(int k = x0; k <= x1; k++)
			rd2ps.plot(k, y, color);
	}
	else
	{
		rd2c* dst = rd2scr.pixels32 + y * rd2scr.width + x0;
		h = x1 - x0;
		do
		{
			rd2ps.blend(dst++, color);
		}
		while(h--);
	}
}

void rd2_vline(rd2i x, rd2i y0, rd2i y1, rd2c color)
{
	rd2i h;
	if(x < 0 || x >= rd2scr.width) return;
	if(y0 > y1) SWAP(y0, y1, h);
	if(y0 < 0) y0 = 0;
	if(y1 >= rd2scr.height)
		y1 = rd2scr.height - 1;
	if(y0 > y1)
		return;

	if(rd2ps.state &= PIXELSHADER_FORCE_PLOT)
	{
		for(int k = y0; k <= y1; k++)
			rd2ps.plot(x, k, color);
	}
	else
	{
		rd2c* dst = rd2scr.pixels32 + y0 * rd2scr.width + x;
		h = y1 - y0;
		uint32 sw = rd2scr.width;
		do
		{
			rd2ps.blend(dst, color);
			dst += sw;
		}
		while(h--);
	}
}

void rd2_line(rd2i x0, rd2i y0, rd2i x1, rd2i y1, rd2c color)
{
	int32 dx = ABS(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int32 dy = -ABS(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int32 err = dx + dy, e2; // error value e_xy

	for(;;)
	{
		rd2ps.plot(x0, y0, color);
		if(x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if(e2 >= dy) { err += dy; x0 += sx; } // e_xy + e_x > 0
		if(e2 <= dx) { err += dx; y0 += sy; } // e_xy + e_y < 0
	}
}

void rd2_circle(rd2i x0, rd2i y0, rd2i r, rd2c color)
{
	int32 E = 1 - r;
	int32 dx = 0;
	int32 dy = -2 * r;
	int32 x = 0;
	int32 y = r;

	rd2ps.plot(x0, y0 + r, color);
	rd2ps.plot(x0, y0 - r, color);
	rd2ps.plot(x0 + r, y0, color);
	rd2ps.plot(x0 - r, y0, color);

	while(x < y - 1)
	{
		x++;

		if(E >= 0)
		{
			y--;
			dy += 2;
			E += dy;
		}

		dx += 2;
		E += dx + 1;

		rd2ps.plot(x0 + x, y0 + y, color);
		rd2ps.plot(x0 - x, y0 + y, color);
		rd2ps.plot(x0 + x, y0 - y, color);
		rd2ps.plot(x0 - x, y0 - y, color);

		if(x != y)
		{
			rd2ps.plot(x0 + y, y0 + x, color);
			rd2ps.plot(x0 - y, y0 + x, color);
			rd2ps.plot(x0 + y, y0 - x, color);
			rd2ps.plot(x0 - y, y0 - x, color);
		}
	}
}

void rd2_circle_fill(rd2i x0, rd2i y0, rd2i r, rd2c color)
{
	if(r <= 0)
	{
		return;
	}

	int32 E = 1 - r;
	int32 dx = 0;
	int32 dy = -2 * r;
	int32 x = 0;
	int32 y = r;

	//rd2_line(x0 - r + 1, y0, x0 + r, y0, color);
	rd2_hline(x0 - r + 1, x0 + r, y0, color);

	while(x < y - 1)
	{
		x++;

		if(E >= 0)
		{
			y--;
			dy += 2;
			E += dy;

			//rd2_line(x0 - x + 1, y0 + y, x0 + x, y0 + y, color);
			//rd2_line(x0 - x + 1, y0 - y, x0 + x, y0 - y, color);
			rd2_hline(x0 - x + 1, x0 + x, y0 + y, color);
			rd2_hline(x0 - x + 1, x0 + x, y0 - y, color);
		}

		dx += 2;
		E += dx + 1;

		if(x != y)
		{
			//rd2_line(x0 - y + 1, y0 + x, x0 + y, y0 + x, color);
			//rd2_line(x0 - y + 1, y0 - x, x0 + y, y0 - x, color);
			rd2_hline(x0 - y + 1, x0 + y, y0 + x, color);
			rd2_hline(x0 - y + 1, x0 + y, y0 - x, color);
		}
	}
}

void rd2_ellipse(rd2i x0, rd2i y0, rd2i w, rd2i h, rd2c color)
{
	rd2i x1 = x0 + w, y1 = y0 + h;

	int32 a = ABS(x1 - x0), b = ABS(y1 - y0), b1 = b & 1; /* values of diameter */

	int64 dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
	int64 err = dx + dy + b1 * a * a, e2; /* error of 1.step */

	if(x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
	if(y0 > y1) y0 = y1; /* .. exchange them */
	y0 += (b + 1) / 2;
	y1 = y0 - b1;   /* starting pixel */
	a *= 8 * a; b1 = 8 * b * b;
	do
	{
		rd2ps.plot(x1, y0, color); /*   I. Quadrant */
		rd2ps.plot(x0, y0, color); /*  II. Quadrant */
		rd2ps.plot(x0, y1, color); /* III. Quadrant */
		rd2ps.plot(x1, y1, color); /*  IV. Quadrant */
		e2 = 2 * err;
		if(e2 >= dx)
		{
			x0++;
			x1--;
			err += dx += b1;
		} /* x step */
		if(e2 <= dy)
		{
			y0++;
			y1--;
			err += dy += a;
		}  /* y step */
	}
	while(x0 <= x1);
	while(y0 - y1 < b)
	{  /* too early stop of flat ellipses a=1 */
		rd2ps.plot(x0 - 1, y0, color); /* -> finish tip of ellipse */
		rd2ps.plot(x1 + 1, y0++, color);
		rd2ps.plot(x0 - 1, y1, color);
		rd2ps.plot(x1 + 1, y1--, color);
	}
}

void rd2_ellipse_fill(rd2i x0, rd2i y0, rd2i w, rd2i h, rd2c color)
{
	rd2i x1 = x0 + w, y1 = y0 + h;

	int32 a = ABS(x1 - x0), b = ABS(y1 - y0), b1 = b & 1; /* values of diameter */

	int64 dx = 4 * (1 - a) * b * b, dy = 4 * (b1 + 1) * a * a; /* error increment */
	int64 err = dx + dy + b1 * a * a, e2; /* error of 1.step */

	if(x0 > x1) { x0 = x1; x1 += a; } /* if called with swapped points */
	if(y0 > y1) y0 = y1; /* .. exchange them */
	y0 += (b + 1) / 2;
	y1 = y0 - b1;   /* starting pixel */
	a *= 8 * a; b1 = 8 * b * b;

	rd2i y0_last = INT32_MIN;
	rd2i y1_last = INT32_MAX;

	do
	{
		if(y0_last != y0)
			rd2_hline(x1, x0, y0, color);

		//rd2_plot(x1, y0, color); /*   I. Quadrant */
		//rd2_plot(x0, y0, color); /*  II. Quadrant */

		if(y1_last != y1 && y0 != y1)
			rd2_hline(x0, x1, y1, color);

		//rd2_plot(x0, y1, color); /* III. Quadrant */
		//rd2_plot(x1, y1, color); /*  IV. Quadrant */

		y0_last = y0;
		y1_last = y1;

		e2 = 2 * err;
		if(e2 >= dx)
		{
			x0++;
			x1--;
			err += dx += b1;
		} /* x step */
		if(e2 <= dy)
		{
			y0++;
			y1--;
			err += dy += a;
		}  /* y step */
	}
	while(x0 <= x1);
	while(y0 - y1 < b)
	{  /* too early stop of flat ellipses a=1 */

		rd2_hline(x0 - 1, x1 + 1, y0, color); y0++;

		//rd2_plot(x0 - 1, y0, color); /* -> finish tip of ellipse */
		//rd2_plot(x1 + 1, y0++, color);

		rd2_hline(x0 - 1, x1 + 1, y1, color); y1--;

		//rd2_plot(x0 - 1, y1, color);
		//rd2_plot(x1 + 1, y1--, color);
	}
}

void rd2_rect(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color)
{
	int32 x1, y1;

	if(w <= 0 || h <= 0)
		return;
	else if(w == 1 && h != 1)
		//rd2_line(x, y - h + 1, x, y + h - 1, color);
		rd2_vline(x, y, y + h - 1, color);
	else if(w != 1 && h == 1)
		//rd2_line(x - w + 1, y, x + w - 1, y, color);
		rd2_hline(x, x + w - 1, y, color);
	else if(w == 1 && h == 1)
		rd2ps.plot(x, y, color);
	else
	{
		x1 = x + w - 1;
		y1 = y + h - 1;

		rd2_hline(x, x1, y, color);
		rd2_hline(x, x1, y1, color);

		rd2_vline(x, y, y1, color);
		rd2_vline(x1, y, y1, color);

		//rd2_line(x, y, x1, y, color);
		//rd2_line(x1, y, x1, y1, color);
		//rd2_line(x1, y1, x, y1, color);
		//rd2_line(x, y1, x, y, color);
	}
}

void rd2_box(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border)
{
	rd2_rect(x, y, w, h, color_border);
	rd2_rect_fill(x + 1, y + 1, w - 2, h - 2, color_background);
}

void rd2_bubble(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border)
{
	if(w > 4 && h > 4)
	{
		w--;
		h--;

		rd2_hline(x + 2, x + w - 2, y, color_border);
		rd2_hline(x + 2, x + w - 2, y + h, color_border);

		rd2_vline(x, y + 2, y + h - 2, color_border);
		rd2_vline(x + w, y + 2, y + h - 2, color_border);

		rd2_plot(x + 1, y + 1, color_border);
		rd2_plot(x + w - 1, y + 1, color_border);
		rd2_plot(x + 1, y + h - 1, color_border);
		rd2_plot(x + w - 1, y + h - 1, color_border);

		rd2_hline(x + 2, x + w - 2, y + 1, color_background);
		rd2_hline(x + 2, x + w - 2, y - 1 + h, color_background);
		rd2_rect_fill(x + 1, y + 2, w - 1, h - 3, color_background);
	}
	else
		rd2_tile(x, y, w, h, color_background, color_border);
}

void rd2_tile(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border)
{
	if(w == 1)
	{
		rd2_vline(x, y, y + h - 1, color_border);
	}
	else if(h == 1)
	{
		rd2_hline(x, x + w - 1, y, color_border);
	}
	else if(w > 0 && h > 0)
	{
		w--;
		h--;

		rd2_hline(x + 1, x + w - 1, y, color_border);
		rd2_hline(x + 1, x + w - 1, y + h, color_border);

		rd2_vline(x, y + 1, y + h - 1, color_border);
		rd2_vline(x + w, y + 1, y + h - 1, color_border);

		rd2_rect_fill(x + 1, y + 1, w - 1, h - 1, color_background);
	}
}

void rd2_poly(rd2i* vxy, int vcount, rd2c color)
{
	for(int i = 0; i < vcount; i++)
	{
		rd2i x0, y0, x1, y1;

		x0 = vxy[i * 2];
		y0 = vxy[i * 2 + 1];

		int i1 = (i + 1) % vcount;

		x1 = vxy[i1 * 2];
		y1 = vxy[i1 * 2 + 1];

		rd2_line(x0, y0, x1, y1, color);
	}
}

void rd2_line_tx0x1(rd2i x0, rd2i y0, rd2i x1, rd2i y1)
{
	if(y0 == y1)
	{
		if(y0 >= 0 && y0 < rd2scr.height)
		{
			tx0x1[y0][0] = MIN(tx0x1[y0][0], x0);
			tx0x1[y0][1] = MAX(tx0x1[y0][1], x0);
			tx0x1[y0][0] = MIN(tx0x1[y0][0], x1);
			tx0x1[y0][1] = MAX(tx0x1[y0][1], x1);
		}
		return;
	}

	int32 dx = ABS(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int32 dy = -ABS(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int32 err = dx + dy, e2; // error value e_xy

	for(;;)
	{
		//rd2_plot(x0, y0, color);
		if(y0 >= 0 && y0 < rd2scr.height)
		{
			tx0x1[y0][0] = MIN(tx0x1[y0][0], x0);
			tx0x1[y0][1] = MAX(tx0x1[y0][1], x0);
		}

		if(x0 == x1 && y0 == y1) break;
		e2 = 2 * err;
		if(e2 >= dy) { err += dy; x0 += sx; } // e_xy + e_x > 0
		if(e2 <= dx) { err += dx; y0 += sy; } // e_xy + e_y < 0
	}
}

void rd2_poly_fill(rd2i* vxy, int vcount, rd2c color)
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

	for(int y = y_min; y <= y_max; y++)
		rd2_hline(tx0x1[y][0], tx0x1[y][1], y, color);
}