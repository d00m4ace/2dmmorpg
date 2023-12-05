//--------------------------------------------------------------------------------------
RD2PIXELSHADER rd2_shader_init(void)
{
	RD2PIXELSHADER ps;
	ps.plot = rd2_plot;

	//ps.blend = rd2_shader_blit;
	//ps.blend = rd2_shader_blit_tint;

	ps.blend = rd2_shader_blend;
	//ps.blend = rd2_shader_blend_tint;

	ps.state = PIXELSHADER_NONE;

	ps.tint = ARGB32(255, 255, 255, 255);

	ps.data = NULL;

	return ps;
}

void rd2_shader_set(RD2PIXELSHADER* ps)
{
	rd2ps = *ps;
}

RD2PIXELSHADER* rd2_shader_get(void)
{
	return &rd2ps;
}
//--------------------------------------------------------------------------------------
void rd2_shader_copy(uint32* dst, uint32 src)
{
	*dst = src;
}

void rd2_shader_blit(uint32* dst, uint32 src)
{
	if(src & 0xF8000000)
		*dst = src;
}

void rd2_shader_blend(uint32* dst, uint32 src)
{
	if(!(src & 0xF8000000))
		return;

	uint32 s_a = ARGB32_GETA(src);
	uint32 s_r = ARGB32_GETR(src);
	uint32 s_g = ARGB32_GETG(src);
	uint32 s_b = ARGB32_GETB(src);

	//s_a = 127;
	s_a = EXPAND256(s_a);

	uint32 dc = *dst;
	//uint32 d_a = ARGB32_GETA(dc);
	uint32 d_r = ARGB32_GETR(dc);
	uint32 d_g = ARGB32_GETG(dc);
	uint32 d_b = ARGB32_GETB(dc);

	//	d_r = (d_r * (256 - s_a) + s_r * s_a) >> 8;
	//	d_g = (d_g * (256 - s_a) + s_g * s_a) >> 8;
	//	d_b = (d_b * (256 - s_a) + s_b * s_a) >> 8;

	d_r += ((s_r - d_r) * s_a) >> 8;
	d_g += ((s_g - d_g) * s_a) >> 8;
	d_b += ((s_b - d_b) * s_a) >> 8;

	src = ARGB32(d_r, d_g, d_b, 255);
	*dst = src;
}

void rd2_shader_blit_tint(uint32* dst, uint32 src)
{
	if(src & 0xF8000000)
	{
		uint32 s_a = ARGB32_GETA(src);

		uint32 dc = rd2ps.tint;
		uint32 d_a = ARGB32_GETA(dc);

		s_a = EXPAND256(s_a);
		d_a = EXPAND256(d_a);

		s_a = (s_a * d_a) >> 8;

		if(s_a & 0xFF8)
		{
			uint32 s_r = ARGB32_GETR(src);
			uint32 s_g = ARGB32_GETG(src);
			uint32 s_b = ARGB32_GETB(src);

			uint32 d_r = ARGB32_GETR(dc);
			uint32 d_g = ARGB32_GETG(dc);
			uint32 d_b = ARGB32_GETB(dc);

			d_r = (s_r * d_r) >> 8;
			d_g = (s_g * d_g) >> 8;
			d_b = (s_b * d_b) >> 8;

			src = ARGB32(d_r, d_g, d_b, 255);
			*dst = src;
		}
	}
}

void rd2_shader_blend_tint(uint32* dst, uint32 src)
{
	if(src & 0xF8000000)
	{
		uint32 s_a = ARGB32_GETA(src);

		uint32 dc = rd2ps.tint;
		uint32 d_a = ARGB32_GETA(dc);

		s_a = EXPAND256(s_a);
		d_a = EXPAND256(d_a);

		s_a = (s_a * d_a) >> 8;

		if(s_a & 0xFF8)
		{
			uint32 s_r = ARGB32_GETR(src);
			uint32 s_g = ARGB32_GETG(src);
			uint32 s_b = ARGB32_GETB(src);

			uint32 d_r = ARGB32_GETR(dc);
			uint32 d_g = ARGB32_GETG(dc);
			uint32 d_b = ARGB32_GETB(dc);

			d_r = (s_r * d_r) >> 8;
			d_g = (s_g * d_g) >> 8;
			d_b = (s_b * d_b) >> 8;

			s_a = UNEXPAND256(s_a);
			src = ARGB32(d_r, d_g, d_b, s_a);
			rd2_shader_blend(dst, src);
		}
	}
}
//--------------------------------------------------------------------------------------