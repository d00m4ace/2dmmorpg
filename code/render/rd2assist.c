//--------------------------------------------------------------------------------------
static RD2PIXELSHADER rd2ps;
static RD2IMG rd2scr;
//--------------------------------------------------------------------------------------
#define CLIP0(CX, X, X2, W) \
    if (X < CX) {           \
        int32 D = CX - X;     \
        W -= D;             \
        X2 += D;            \
        X += D;             \
    }
#define CLIP1(X, DW, W) \
    if (X + W > DW)     \
        W = DW - X;
#define CLIP()                  \
    CLIP0(0, dx, sx, w);  \
    CLIP0(0, dy, sy, h);  \
    CLIP0(0, sx, dx, w);        \
    CLIP0(0, sy, dy, h);        \
    CLIP1(dx, 0 + cw, w); \
    CLIP1(dy, 0 + ch, h); \
    CLIP1(sx, src->width, w);       \
    CLIP1(sy, src->height, h);       \
    if (w <= 0 || h <= 0)       \
    return
//--------------------------------------------------------------------------------------