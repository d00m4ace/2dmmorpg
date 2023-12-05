void rd2_spr(RD2IMG* src, rd2i x, rd2i y); // support only scaling no rotation
void rd2_spr_fast(RD2IMG* src, rd2i x, rd2i y, float scale); // support only scaling no rotation
void rd2_spr_r(RD2IMG* src, rd2i x, rd2i y, float scale, rd2i rc_x, rd2i rc_y, float radians); // support scaling and rotation
void rd2_spr_rc(RD2IMG* src, rd2i x, rd2i y, float scale, float radians); // rotate around the center of the sprite and support scaling and rotation

void rd2_spr_blit(RD2IMG* src, rd2i dx, rd2i dy, rd2i sx, rd2i sy, rd2i w, rd2i h);

void rd2_spr_quad(RD2IMG* src, rd2i* vxy, int vcount); // support scaling and rotation
void rd2_spr_quad_fast(RD2IMG* src, rd2i* vxy, int vcount); // support only scaling no rotation
