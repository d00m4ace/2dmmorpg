//--------------------------------------------------------------------------------------
typedef struct RD2IMG
{
	uint32* pixels32;
	int32 width, height;
} RD2IMG;

typedef struct c_vec_of(RD2IMG) c_vec_img;
//--------------------------------------------------------------------------------------
RD2IMG rd2_img_init(int32 width, int32 height);
void rd2_img_free(RD2IMG* img);
// direct image operations no shaders!
//--------------------------------------------------------------------------------------
void rd2_img_raw_copy(RD2IMG* img, void* raw_data, int32 raw_bytes_per_line, int32 raw_width, int32 raw_height);
void rd2_raw_img_copy(RD2IMG* img, void* raw_data, int32 raw_bytes_per_line, int32 raw_width, int32 raw_height);

void rd2_img_blit(RD2IMG* dst, RD2IMG* src, rd2i dx, rd2i dy, rd2i sx, rd2i sy, rd2i w, rd2i h); // blit with transparent cut on A less than 8
void rd2_img_copy(RD2IMG* dst, RD2IMG* src);
void rd2_img_color_replace(RD2IMG* dst, rd2c* origin_colors, rd2c* replace_colors, int colors);
void rd2_img_color_match_replace(RD2IMG* dst, rd2c* match_colors, int colors);

void rd2_img_fill(RD2IMG* dst, rd2i x, rd2i y, rd2i w, rd2i h, rd2c color);

void rd2_img_rotate90(RD2IMG* dst, RD2IMG* src);
void rd2_img_hflip(RD2IMG* src);
void rd2_img_vflip(RD2IMG* src);

bool rd2_img_is_vblank(RD2IMG* src, rd2i x);
bool rd2_img_is_vequal(RD2IMG* src, rd2i x0, rd2i x1);
void rd2_img_vcut(RD2IMG* src, rd2i x0);

void rd2_img_y_shift(RD2IMG* dst, RD2IMG* src, int32 y_offs, int32 y_shift);
//--------------------------------------------------------------------------------------
void rd2_img_stack_init(void);

RD2IMG rd2_img_stack_tmp(int32 width, int32 height, rd2c color);
//--------------------------------------------------------------------------------------
