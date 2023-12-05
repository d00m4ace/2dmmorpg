//----------------------------------------------------------------------------------
typedef int32 rd2i; // 32bit scalar coordinate
typedef uint32 rd2c; // 32bit color
//----------------------------------------------------------------------------------
#define DU2(DIG,X) union { int##DIG X; uint##DIG u##X; }

typedef struct { DU2(8, x); DU2(8, y); } point8;
typedef struct { DU2(16, x); DU2(16, y); } point16;
typedef struct { DU2(32, x); DU2(32, y); } point32;

typedef struct { DU2(8, x); DU2(8, y); DU2(8, w); DU2(8, h); } box8;
typedef struct { DU2(16, x); DU2(16, y); DU2(16, w); DU2(16, h); } box16;
typedef struct { DU2(32, x); DU2(32, y); DU2(32, w); DU2(32, h); } box32;
//----------------------------------------------------------------------------------
typedef point32 p32;
//----------------------------------------------------------------------------------
// vector ops
D2_INLINE p32 p32_add(p32 a, p32 b) { a.x += b.x; a.y += b.y; return a; }
D2_INLINE p32 p32_sub(p32 a, p32 b) { a.x -= b.x; a.y -= b.y; return a; }
D2_INLINE p32 p32_invers(p32 a) { a.x = -a.x; a.y = -a.y; return a; }

D2_INLINE p32 p32_max(p32 a, p32 b) { a.x = MAX(a.x, b.x); a.y = MAX(a.y, b.y); return a; }
D2_INLINE p32 p32_min(p32 a, p32 b) { a.x = MIN(a.x, b.x); a.y = MIN(a.y, b.y); return a; }

D2_INLINE p32 p32_from_d2v(d2v v) { return (p32) { v.x, v.y }; }
D2_INLINE d2v p32_to_d2v(p32 p) { return (d2v) { p.x, p.y }; }

D2_INLINE p32 p32_abs(p32 p) { return (p32) { ABS(p.x), ABS(p.y) }; }

D2_INLINE bool p32_is_greater_one(p32 a, p32 b) { return (a.x > b.x || a.y > b.y); }
D2_INLINE bool p32_is_mod_greater_one(p32 a, int32 b) { return p32_is_greater_one(p32_abs(a), (p32) { b, b }); }
//----------------------------------------------------------------------------------
//axis aligned bounding box
typedef struct { p32 min, max; } aabb32;
D2_INLINE aabb32 aabb32_init(void) { return (aabb32) { INT32_MAX, INT32_MAX, INT32_MIN, INT32_MIN }; }
D2_INLINE aabb32 aabb32_from_ps(p32 pos, p32 size) { return (aabb32) { pos.x, pos.y, pos.x + size.x, pos.y + size.y }; }
D2_INLINE aabb32 aabb32_from_p(p32 p) { return (aabb32) { 0, 0, p.x, p.y }; }

D2_INLINE aabb32 aabb32_add(aabb32 a, aabb32 b) { return (aabb32) { MIN(a.min.x, b.min.x), MIN(a.min.y, b.min.y), MAX(a.max.x, b.max.x), MAX(a.max.y, b.max.y) }; }

D2_INLINE p32 aabb32_size(aabb32 a) { return (p32) { a.max.x - a.min.x, a.max.y - a.min.y }; }
D2_INLINE p32 aabb32_pos(aabb32 a) { return (p32) { a.min.x, a.min.y }; }

D2_INLINE bool aabb32_to_p32(aabb32 a, p32 b) { return (b.x >= a.min.x && b.x < a.max.x&& b.y >= a.min.y && b.y < a.max.y); }
D2_INLINE bool aabb32_to_aabb32(aabb32 a, aabb32 b)
{
	int d0 = b.max.x < a.min.x;
	int d1 = a.max.x < b.min.x;
	int d2 = b.max.y < a.min.y;
	int d3 = a.max.y < b.min.y;
	return !(d0 | d1 | d2 | d3);
}
//----------------------------------------------------------------------------------