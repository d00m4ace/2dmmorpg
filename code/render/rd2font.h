#define RD2FONT_MAX_CHAR 256

typedef struct RD2FONT
{
	RD2IMG ci[RD2FONT_MAX_CHAR]; // char images
	int32 cw[RD2FONT_MAX_CHAR]; // char width

	RD2IMG citmp[2];

	int32 char_width;
	int32 char_height;
	int32 space_width;
	int32 padding_width;
	int32 padding_height;

	void* data;
} RD2FONT;

// font print routine
#define FNT_FLG_NONE (0)
#define FNT_FLG_DRAW (1<<0)
#define FNT_FLG_WORD_WRAP (1<<1)

// #! - flash on/off
// ## - print #
// #digit* - repeat next print token digit times
// #digit$ - set current paper pal_color = digit, if digit < 0 pal_color = paper from print arg, see PALETTE_COLORS
// #digit@ - set current ink pal_color = digit, if digit < 0 pal_color = ink from print arg, see PALETTE_COLORS
// #digit& - print game image by id = digit => game_imgs_get(digit)
point32 rd2_font_print(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale, rd2c paper, rd2c ink, uint32 fnt_flgs);

int32 rd2_font_draw_str(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale, rd2c paper, rd2c ink);
int32 rd2_font_draw_str_calc_width(RD2FONT* fnt, const char* txt, rd2i x, rd2i y, float scale);
