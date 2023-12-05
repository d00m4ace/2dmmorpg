void rd2_cls(rd2c color);

void rd2_plot(rd2i x, rd2i y, rd2c color);

void rd2_hline(rd2i x0, rd2i x1, rd2i y, rd2c color);
void rd2_vline(rd2i x, rd2i y0, rd2i y1, rd2c color);

void rd2_line(rd2i x0, rd2i y0, rd2i x1, rd2i y1, rd2c color);

void rd2_circle(rd2i x0, rd2i y0, rd2i radius, rd2c color);
void rd2_ellipse(rd2i x0, rd2i y0, rd2i w, rd2i h, rd2c color);
void rd2_rect(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color);

void rd2_circle_fill(rd2i x0, rd2i y0, rd2i radius, rd2c color);
void rd2_ellipse_fill(rd2i x0, rd2i y0, rd2i w, rd2i h, rd2c color);
void rd2_rect_fill(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color);

void rd2_box(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border);
void rd2_tile(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border);
void rd2_bubble(rd2i x, rd2i y, rd2i w, rd2i h, rd2c color_background, rd2c color_border);

void rd2_poly(rd2i* vxy, int vcount, rd2c color);
void rd2_poly_fill(rd2i* vxy, int vcount, rd2c color);
