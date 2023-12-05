// adjust these primitives as seen fit
#include <string.h> // memcpy
#include <math.h>

typedef float d2f;

#define D2_INFINITY INFINITY
#define D2_NAN NAN

#define D2_EPS      0.01f        // small epsilon 0.01f
#define D2_EPS3     0.001f       // small epsilon 0.001f
#define D2_EPS4     0.0001f      // small epsilon 0.0001f

#define D2_E        2.71828183f  // e
#define D2_LOG2E    1.44269504f  // log2(e)
#define D2_LOG10E   0.43429448f  // log10(e)
#define D2_LN2      0.69314718f  // ln(2)
#define D2_LN10     2.30258509f  // ln(10)
#define D2_PI       3.14159265f  // pi

#define D2_DEG2RAD  (D2_PI/180)
#define D2_RAD2DEG  (180/D2_PI)

#define d2_sin(radians) sinf(radians)
#define d2_cos(radians) cosf(radians)
#define d2_acos(c) acosf(c)
#define d2_asin(s) asinf(s)
#define d2_atan2(x,y) atan2f(x,y)

#define d2_sqrt(a) sqrtf(a)

#define d2_min(a, b) ((a) < (b) ? (a) : (b))
#define d2_max(a, b) ((a) > (b) ? (a) : (b))

#define d2_abs(a) ((a) < 0 ? -(a) : (a))
#define d2_clamp(a, lo, hi) d2_max(lo, d2_min(a, hi))
#define d2_sign(a) (a < 0 ? -1.0f : 1.0f)

D2_INLINE int32 d2_round(d2f a) { return (int32)(a < 0 ? a - 0.5f : a + 0.5f); }

D2_INLINE void d2_sincos(d2f radians, d2f* s, d2f* c) { *c = d2_cos(radians); *s = d2_sin(radians); }

// 2d vector
typedef struct d2v
{
	union
	{
		d2f x;
		d2f c;
	};

	union
	{
		d2f y;
		d2f s;
	};
} d2v;

typedef d2v d2r;

// 2d rotation matrix
typedef struct d2m
{
	union
	{
		d2v x;
		d2v p;
	};

	union
	{
		d2v y;
		d2r r;
	};
} d2m;

typedef d2m d2t;

// 2d halfspace (aka plane, aka line)
typedef struct d2h
{
	d2v n; // normal, normalized
	d2f d; // distance to origin from plane, or ax + by = d
} d2h;

typedef struct d2circle
{
	d2v p;
	d2f r;
} d2circle;

typedef struct d2aabb
{
	d2v min;
	d2v max;
} d2aabb;

// a capsule is defined as a line segment (from a to b) and radius r
typedef struct d2capsule
{
	d2v a;
	d2v b;
	d2f r;
} d2capsule;

// this can be adjusted as necessary, but is highly recommended to be kept at 8.
// higher numbers will incur quite a bit of memory overhead, and convex shapes
// over 8 verts start to just look like spheres, which can be implicitly rep-
// resented as a point + radius. usually tools that generate polygons should be
// constructed so they do not output polygons with too many verts.
// Note: polygons in cute_c2 are all *convex*.
#define D2_MAX_POLYGON_VERTS 8

typedef struct d2poly
{
	int count;
	d2v verts[D2_MAX_POLYGON_VERTS];
	d2v norms[D2_MAX_POLYGON_VERTS];
} d2poly;

// IMPORTANT:
// Many algorithms in this file are sensitive to the magnitude of the ray direction (d2ray::d). 
// It is highly recommended to normalize the ray direction and use t to specify a distance. 
typedef struct d2ray
{
	d2v p; // position
	d2v d; // direction (normalized)
	d2f t; // distance along d from position p to find endpoint of ray
} d2ray;

typedef struct d2raycast
{
	d2f t; // time of impact
	d2v n;   // normal of surface at impact (unit length)
} d2raycast;

// position of impact p = ray.p + ray.d * raycast.t
#define d2_impact(ray, t) d2v_add(ray.p, d2v_mul(ray.d, t))

// contains all information necessary to resolve a collision, or in other words
// this is the information needed to separate shapes that are colliding. 
typedef struct d2manifold
{
	int count;
	d2f depths[2];
	d2v contact_points[2];

	// always points from shape A to shape B (first and second shapes passed into any of the d2***to***manifold functions)
	d2v n;
} d2manifold;

// vector ops
D2_INLINE d2v d2v_set(d2f x, d2f y) { d2v a; a.x = x; a.y = y; return a; }
D2_INLINE d2v d2v_add(d2v a, d2v b) { a.x += b.x; a.y += b.y; return a; }
D2_INLINE d2v d2v_sub(d2v a, d2v b) { a.x -= b.x; a.y -= b.y; return a; }

// 1. The dot product is commutative! dot(a, b) == dot(b, a).
// 2. You can algebraically manipulate scalars in and out of dot products.Example: given two vectors a and b, and a scalar s, we can see : s * dot(a, b) == dot(s * a, s * b).
// 3. Given two vectors u and v, if u is unit(length of 1), then dot(u, v) == the distance v travels along u.
// 4. If two vectors u and v are unit vectors, dot(u, v) == cos(angle between u and v).
// 5. Given two vectors u and v, whether or not they are unit, sign(dot(u, v)) will tell you if the vectors are facing each other or away from each other.
// 6. The dot product of perpendicular vectors is 0.
D2_INLINE d2f d2v_dot(d2v a, d2v b) { return a.x * b.x + a.y * b.y; }

D2_INLINE d2v d2v_mul(d2v a, d2f b) { a.x *= b; a.y *= b; return a; }
D2_INLINE d2v d2v_mul_v(d2v a, d2v b) { a.x *= b.x; a.y *= b.y; return a; }
D2_INLINE d2v d2v_div(d2v a, d2f b) { return d2v_mul(a, 1.0f / b); }

//skew returns vector rotated by 90 degrees.
D2_INLINE d2v d2v_skew(d2v a) { d2v b; b.x = -a.y; b.y = a.x; return b; }
//rotation by 90 degrees in the opposite direction to skew.
D2_INLINE d2v d2v_ccw90(d2v a) { d2v b; b.x = a.y; b.y = -a.x; return b; }

// Cross product aka 2D determinant:
// 1. Computes the sin of the angle between two vectors (scaled by the length of each vector).
// 2. The cross between two parallel vectors is 0.
D2_INLINE d2f d2v_det2(d2v a, d2v b) { return a.x * b.y - a.y * b.x; }
#define d2v_cross(a,b) d2v_det2(a,b)

D2_INLINE d2v d2v_min(d2v a, d2v b) { return d2v_set(d2_min(a.x, b.x), d2_min(a.y, b.y)); }
D2_INLINE d2v d2v_max(d2v a, d2v b) { return d2v_set(d2_max(a.x, b.x), d2_max(a.y, b.y)); }
D2_INLINE d2v d2v_clamp(d2v a, d2v lo, d2v hi) { return d2v_max(lo, d2v_min(a, hi)); }
D2_INLINE d2v d2v_abs(d2v a) { return d2v_set(d2_abs(a.x), d2_abs(a.y)); }
D2_INLINE d2f d2v_hmin(d2v a) { return d2_min(a.x, a.y); }
D2_INLINE d2f d2v_hmax(d2v a) { return d2_max(a.x, a.y); }

D2_INLINE d2f d2v_len(d2v a) { return d2_sqrt(d2v_dot(a, a)); }
//d2v_len2 aka length squared function
D2_INLINE d2f d2v_len2(d2v a) { return d2v_dot(a, a); }

D2_INLINE d2v d2v_norm(d2v a) { return d2v_div(a, d2v_len(a)); }
D2_INLINE d2v d2v_safe_norm(d2v a) { d2f sq = d2v_dot(a, a); return sq ? d2v_div(a, d2v_len(a)) : d2v_set(0, 0); }

D2_INLINE d2v d2v_neg(d2v a) { return d2v_set(-a.x, -a.y); }

D2_INLINE d2v d2v_lerp(d2v a, d2v b, d2f t) { return d2v_add(a, d2v_mul(d2v_sub(b, a), t)); }

//atan2, the function used to compute the angle from the x-axis to a vector, returns a number from pi to -pi
//it’s often helpful to remap this from 0 to 2 * pi by adding pi to the result of atan2.
//d2r r = d2r_set(d2v_angle(v));
D2_INLINE d2f d2v_angle(d2v v) { return d2_atan2(-v.y, -v.x) + D2_PI; }

// Reflect vector d across vector n. See: http://paulbourke.net/geometry/reflected/
// v2 reflect(v2 d, v2 n) { return d - n * 2 * dot(d, n); }
D2_INLINE d2v d2v_reflect(d2v d, d2v n) { return d2v_sub(d, d2v_mul(n, 2 * d2v_dot(d, n))); }

D2_INLINE int d2v_parallel(d2v a, d2v b, d2f eps)
{
	d2f k = d2v_len(a) / d2v_len(b);
	b = d2v_mul(b, k);
	if(d2_abs(a.x - b.x) < eps && d2_abs(a.y - b.y) < eps) return 1;
	return 0;
}

D2_INLINE d2v d2v_project(d2v v0, d2v v1)
{
	d2f d = d2v_dot(v1, v1);
	d2f s = d2v_dot(v0, v1) / d;
	d2v prj = d2v_mul(v1, s);
	return prj;
}

D2_INLINE d2v d2v_project_l(d2v v1, d2v v2, d2v p)
{
	// get dot product of e1, e2
	d2v e1 = d2v_sub(v2, v1);
	d2v e2 = d2v_sub(p, v1);
	d2f valDp = d2v_dot(e1, e2);
	// get squared length of e1
	d2f len2 = d2v_len2(e1);
	d2v prj = d2v_set((v1.x + (valDp * e1.x) / len2), (v1.y + (valDp * e1.y) / len2));
	return prj;
}

// rotation ops
D2_INLINE d2r d2r_set(d2f radians) { d2r r; d2_sincos(radians, &r.s, &r.c); return r; }
D2_INLINE d2r d2r_identity(void) { d2r r; r.c = 1.0f; r.s = 0; return r; }
D2_INLINE d2v d2r_rot_x(d2r r) { return d2v_set(r.c, r.s); }
D2_INLINE d2v d2r_rot_y(d2r r) { return d2v_set(-r.s, r.c); }
D2_INLINE d2v d2r_mul_v(d2r a, d2v b) { return d2v_set(a.c * b.x - a.s * b.y, a.s * b.x + a.c * b.y); }
D2_INLINE d2v d2r_mul_vt(d2r a, d2v b) { return d2v_set(a.c * b.x + a.s * b.y, -a.s * b.x + a.c * b.y); }
D2_INLINE d2r d2r_mul_r(d2r a, d2r b) { d2r c; c.c = a.c * b.c - a.s * b.s; c.s = a.s * b.c + a.c * b.s; return c; }
D2_INLINE d2r d2r_mul_rt(d2r a, d2r b) { d2r c; c.c = a.c * b.c + a.s * b.s; c.s = a.c * b.s - a.s * b.c; return c; }
D2_INLINE d2f d2r_angle(d2r r) { return d2v_angle(r); }
D2_INLINE int d2r_parallel(d2r a, d2r b, d2f eps) { if(d2_abs(d2v_det2(a, b)) < eps) return 1; return 0; }

D2_INLINE d2v d2m_mul_v(d2m a, d2v b) { d2v c; c.x = a.x.x * b.x + a.y.x * b.y; c.y = a.x.y * b.x + a.y.y * b.y; return c; }
D2_INLINE d2v d2m_mul_vt(d2m a, d2v b) { d2v c; c.x = a.x.x * b.x + a.x.y * b.y; c.y = a.y.x * b.x + a.y.y * b.y; return c; }
D2_INLINE d2m d2m_mul(d2m a, d2m b) { d2m c; c.x = d2m_mul_v(a, b.x);  c.y = d2m_mul_v(a, b.y); return c; }
D2_INLINE d2m d2m_mul_t(d2m a, d2m b) { d2m c; c.x = d2m_mul_vt(a, b.x); c.y = d2m_mul_vt(a, b.y); return c; }

// halfspace ops
D2_INLINE d2h d2h_set(d2v n, d2v p) { d2h h; h.n = n; h.d = d2v_dot(n, p); return h; }
D2_INLINE d2v d2h_origin(d2h h) { return d2v_mul(h.n, h.d); }
D2_INLINE d2f d2h_dist(d2h h, d2v p) { return d2v_dot(h.n, p) - h.d; }
D2_INLINE d2v d2h_project(d2h h, d2v p) { return d2v_sub(p, d2v_mul(h.n, d2h_dist(h, p))); }
D2_INLINE int d2h_parallel(d2h a, d2h b, d2f eps) { return d2r_parallel(a.n, b.n, eps); }

D2_INLINE d2v d2h_intersect(d2h a, d2h b)
{
	return d2v_add(a.n, d2v_mul(d2v_sub(b.n, a.n), (a.d / (a.d - b.d))));
}

// line: p + q * t, if t = 0.0 .. 1.0 has a hit, t = D2_NAN if parallel
// plane = ax + by - c = 0, or in vector form dot(n, q) - d = 0 where q is an input point
D2_INLINE d2f d2h_intersect_l(d2h h, d2v p, d2v q)
{
	d2f dot_hnq = d2v_dot(h.n, q);

	if(d2_abs(dot_hnq) < D2_EPS3)
		return D2_NAN;

	d2f t = (h.d - d2v_dot(h.n, p)) / dot_hnq;
	return t;
}

// A quadratic bezier curve is just lerping between two other lerps.
D2_INLINE d2v d2v_bezier(d2v a, d2v b, d2v c, d2f t)
{
	//d2v d = d2v_lerp(a, b, t);
	//d2v e = d2v_lerp(b, c, t);
	//return d2v_lerp(d, e, t);
	// optimization: 
	d2f u = 1.0f - t;
	d2f ut = u * t;
	d2v auu = d2v_mul(a, u * u);
	d2v but2 = d2v_mul(b, ut * 2.0f);
	d2v ctt = d2v_mul(c, t * t);
	return d2v_add(auu, d2v_add(but2, ctt));
}

// A cubic bezier curve is just lerping between two quadratic bezier curves.
D2_INLINE d2v d2v_bezier3(d2v a, d2v b, d2v c, d2v d, d2f t)
{
	//d2v e = d2v_bezier(a, b, c, t);
	//d2v f = d2v_bezier(b, c, d, t);
	//return d2v_lerp(e, f, t);
	// optimization:
	d2f u = 1.0f - t;
	d2f tt = t * t;
	d2f uu = u * u;
	d2v auuu = d2v_mul(a, uu * u);
	d2v buut3 = d2v_mul(b, uu * t * 3.0f);
	d2v cutt3 = d2v_mul(c, u * tt * 3.0f);
	d2v dttt = d2v_mul(d, tt * t);
	return d2v_add(auuu, d2v_add(buut3, d2v_add(cutt3, dttt)));
}

// transform ops
// These are used especially for d2poly when a d2poly is passed to a function.
// Since polygons are prime for "instancing" a d2m transform can be used to
// transform a polygon from local space to world space. In functions that take
// a d2m pointer (like d2poly_to_poly), these pointers can be NULL, which represents
// an identity transformation and assumes the verts inside of d2poly are already
// in world space.
D2_INLINE d2t d2t_set(d2v p, d2f radians) { d2t x; x.r = d2r_set(radians); x.p = p; return x; }
D2_INLINE d2t d2t_identity(void) { d2t x; x.p = d2v_set(0, 0); x.r = d2r_identity(); return x; }
D2_INLINE d2v d2t_mul_v(d2t a, d2v b) { return d2v_add(d2r_mul_v(a.r, b), a.p); }
D2_INLINE d2v d2t_mul_vt(d2t a, d2v b) { return d2r_mul_vt(a.r, d2v_sub(b, a.p)); }
D2_INLINE d2t d2t_mul(d2t a, d2t b) { d2t c; c.r = d2r_mul_r(a.r, b.r); c.p = d2v_add(d2r_mul_v(a.r, b.p), a.p); return c; }
D2_INLINE d2t d2t_mul_t(d2t a, d2t b) { d2t c; c.r = d2r_mul_rt(a.r, b.r); c.p = d2r_mul_vt(a.r, d2v_sub(b.p, a.p)); return c; }
D2_INLINE d2h d2t_mul_h(d2t a, d2h b) { d2h c; c.n = d2r_mul_v(a.r, b.n); c.d = d2v_dot(d2t_mul_v(a, d2h_origin(b)), c.n); return c; }
D2_INLINE d2h d2t_mul_ht(d2t a, d2h b) { d2h c; c.n = d2r_mul_vt(a.r, b.n); c.d = d2v_dot(d2t_mul_vt(a, d2h_origin(b)), c.n); return c; }

D2_INLINE d2f d2v_norm_short_arc(d2v a, d2v b)
{
	d2f theta = d2_acos(d2v_dot(a, b));
	if(d2v_det2(a, b) > 0) return theta;
	else return -theta;
}

//d2v aim; d2f angle = d2v_short_arc(aim, m);
//aim = mul(sincos(angle * dt * 2.0f), aim);
D2_INLINE d2f d2v_short_arc(d2v a, d2v b)
{
	return d2v_norm_short_arc(d2v_norm(a), d2v_norm(b));
}

D2_INLINE void d2aabb_to_v4(d2v* out, d2aabb* bb)
{
	out[0] = bb->min;
	out[1] = d2v_set(bb->max.x, bb->min.y);
	out[2] = bb->max;
	out[3] = d2v_set(bb->min.x, bb->max.y);
}

// boolean collision detection
// these versions are faster than the manifold versions, but only give a YES/NO result
int d2circle_to_circle(d2circle A, d2circle B);
int d2circle_to_aabb(d2circle A, d2aabb B);
int d2circle_to_capsule(d2circle A, d2capsule B);

int d2aabb_to_aabb(d2aabb A, d2aabb B);
int d2aabb_to_capsule(d2aabb A, d2capsule B);

int d2capsule_to_capsule(d2capsule A, d2capsule B);

int d2circle_to_poly(d2circle A, const d2poly* B, const d2t* bx);
int d2aabb_to_poly(d2aabb A, const d2poly* B, const d2t* bx);
int d2capsule_to_poly(d2capsule A, const d2poly* B, const d2t* bx);
int d2poly_to_poly(const d2poly* A, const d2t* ax, const d2poly* B, const d2t* bx);

int d2aabb_to_point(d2aabb A, d2v B);
int d2circle_to_point(d2circle A, d2v B);

// ray operations
// output is placed into the d2raycast struct, which represents the hit location
// of the ray. the out param contains no meaningful information if these funcs
// return 0
int d2ray_to_circle(d2ray A, d2circle B, d2raycast* out);
int d2ray_to_aabb(d2ray A, d2aabb B, d2raycast* out);
int d2ray_to_capsule(d2ray A, d2capsule B, d2raycast* out);
int d2ray_to_poly(d2ray A, const d2poly* B, const d2t* bx, d2raycast* out);

// manifold generation
// These functions are (generally) slower than the boolean versions, but will compute one
// or two points that represent the plane of contact. This information is usually needed
// to resolve and prevent shapes from colliding. If no collision occured the count member
// of the manifold struct is set to 0.
void d2circle_to_circle_manifold(d2circle A, d2circle B, d2manifold* m);
void d2circle_to_aabb_manifold(d2circle A, d2aabb B, d2manifold* m);
void d2circle_to_capsule_manifold(d2circle A, d2capsule B, d2manifold* m);

void d2aabb_to_aabb_manifold(d2aabb A, d2aabb B, d2manifold* m);
void d2aabb_to_capsule_manifold(d2aabb A, d2capsule B, d2manifold* m);

void d2capsule_to_capsule_manifold(d2capsule A, d2capsule B, d2manifold* m);

void d2circle_to_poly_manifold(d2circle A, const d2poly* B, const d2t* bx, d2manifold* m);
void d2aabb_to_poly_manifold(d2aabb A, const d2poly* B, const d2t* bx, d2manifold* m);
void d2capsuleto_poly_manifold(d2capsule A, const d2poly* B, const d2t* bx, d2manifold* m);
void d2poly_to_poly_manifold(const d2poly* A, const d2t* ax, const d2poly* B, const d2t* bx, d2manifold* m);

int d2poly_hull(d2v* verts, int count);
void d2poly_norms(d2v* verts, d2v* norms, int count);
void d2poly_make(d2poly* p);

// TODO: CHECK!
//p + d * t = q
//p.x + d.x * t = q.x
//p.y + d.y * t = q.y
//t = (q.x - p.x) / d.x
//t = (q.y - p.y) / d.y
// if u is unit(length of 1), then dot(u, v) == the distance v travels along u.
// t = d2v_dot(d,d2v_sub(q,p))