RD2FONT rd2_font_init(int32 width, int32 height);
void rd2_font_free(RD2FONT* fnt);

void rd2_font8_set(RD2FONT* dst, char* data, int char_count);

RD2FONT rd2_font_border(RD2FONT* fnt, int border, rd2c color_border);

bool rd2_font_save_image(RD2FONT* font8bit, const char* file_name);
bool rd2_font_load_image(RD2FONT* fnt, const char* file_name);

void rd2_font_trim_chars(RD2FONT* fnt);
void rd2_font_thin_chars(RD2FONT* fnt, int vequal_max);