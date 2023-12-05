void rd2_scr_set(RD2IMG* img);
RD2IMG* rd2_scr_get(void);

int32 rd2_scr_width(void);
int32 rd2_scr_height(void);

void rd2_scr_draw(int32 x, int32 y, int32 width_scale, int32 height_scale);

void rd2_scr_stack_init(void);
void rd2_scr_stack_push(int32 width, int32 height, rd2c color);
RD2IMG rd2_scr_stack_pop(void);
void rd2_scr_stack_pop_show(p32 offs);
