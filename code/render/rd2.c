//--------------------------------------------------------------------------------------
void rd2_scr_set(RD2IMG* scr)
{
	rd2scr = *scr;
}

RD2IMG* rd2_scr_get(void)
{
	return &rd2scr;
}

int32 rd2_scr_width(void)
{
	return rd2scr.width;
}
int32 rd2_scr_height(void)
{
	return rd2scr.height;
}
//--------------------------------------------------------------------------------------
void rd2_scr_draw(int32 x, int32 y, int32 width_scale, int32 height_scale)
{
	uint32* dst_pixels = hal_screen_pixels();
	int32 dst_pixels_per_line = hal_screen_max_size();

	for(int h = 0; h < rd2scr.height; h++)
	{
		uint32* dst = dst_pixels + h * dst_pixels_per_line;
		uint32* scr = rd2scr.pixels32 + h * rd2scr.width;
		for(int w = 0; w < rd2scr.width; w++)
			*dst++ = *scr++;
	}

	hal_update_screen(rd2scr.height);

	hal_draw_screen(x, y, rd2scr.width, rd2scr.height, width_scale, height_scale);
}
//--------------------------------------------------------------------------------------
#define RD2_SCR_STACK_MAX_SIZE 8

static RD2IMG rd2_scr_stack[RD2_SCR_STACK_MAX_SIZE] = { 0 };
static RD2IMG rd2_scr_stack_save[RD2_SCR_STACK_MAX_SIZE] = { 0 };
static RD2PIXELSHADER rd2_ps_stack_save[RD2_SCR_STACK_MAX_SIZE] = { 0 };
static int32 rd2_scr_stack_count = 0;

void rd2_scr_stack_init(void)
{
	rd2_scr_stack_count = 0;

	for(int i = 0; i < RD2_SCR_STACK_MAX_SIZE; i++)
	{
		rd2_img_free(&rd2_scr_stack[i]);
		rd2_scr_stack[i] = rd2_img_init(game_scr_width(), game_scr_height());
	}
}

void rd2_scr_stack_push(int32 width, int32 height, rd2c color)
{
	ASSERT(rd2_scr_stack_count < RD2_SCR_STACK_MAX_SIZE);

	rd2_scr_stack_save[rd2_scr_stack_count] = rd2scr;
	rd2_ps_stack_save[rd2_scr_stack_count] = rd2ps;

	rd2scr = rd2_scr_stack[rd2_scr_stack_count];
	rd2ps = rd2_shader_init();

	rd2scr.width = MIN(rd2scr.width, width);
	rd2scr.height = MIN(rd2scr.height, height);
	rd2_cls(color);

	rd2_scr_stack_count++;
}

RD2IMG rd2_scr_stack_pop(void)
{
	ASSERT(rd2_scr_stack_count > 0);

	RD2IMG img = rd2scr;

	rd2_scr_stack_count--;

	rd2scr = rd2_scr_stack_save[rd2_scr_stack_count];
	rd2ps = rd2_ps_stack_save[rd2_scr_stack_count];

	return img;
}

void rd2_scr_stack_pop_show(p32 offs)
{
	RD2IMG img = rd2_scr_stack_pop();
	rd2_spr_blit(&img, offs.x, offs.y, 0, 0, img.width, img.height);
}
//--------------------------------------------------------------------------------------