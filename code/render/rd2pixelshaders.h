//--------------------------------------------------------------------------------------
#define PIXELSHADER_NONE 0
#define PIXELSHADER_FORCE_PLOT 1

typedef struct RD2PIXELSHADER
{
	void (*plot)(rd2i x, rd2i y, rd2c color);
	void (*blend)(uint32* dst, uint32 src);
	int32 state;
	rd2c tint;
	void* data;
} RD2PIXELSHADER;
//--------------------------------------------------------------------------------------
RD2PIXELSHADER rd2_shader_init(void);
void rd2_shader_set(RD2PIXELSHADER* ps);
RD2PIXELSHADER* rd2_shader_get(void);
//--------------------------------------------------------------------------------------
void rd2_shader_copy(uint32* dst, uint32 src);

void rd2_shader_blit(uint32* dst, uint32 src); // blit with transparent cut on A less than 8
void rd2_shader_blit_tint(uint32* dst, uint32 src); // blit with transparent cut on A less than 8

void rd2_shader_blend(uint32* dst, uint32 src); // blit with transparent cut on A less than 8
void rd2_shader_blend_tint(uint32* dst, uint32 src); // blit with transparent cut on A less than 8
//--------------------------------------------------------------------------------------