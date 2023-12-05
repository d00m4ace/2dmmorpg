#define CUTE_C2_IMPLEMENTATION
#include "cute_c2.h"

int d2poly_hull(d2v* verts, int count)
{
	return c2Hull(verts, count);
}

void d2poly_norms(d2v* verts, d2v* norms, int count)
{
	c2Norms(verts, norms, count);
}

void d2poly_make(d2poly* p)
{
	c2MakePoly(p);
}

// boolean collision detection
// these versions are faster than the manifold versions, but only give a YES/NO result
int d2circle_to_circle(d2circle A, d2circle B)
{
	c2v c = d2v_sub(B.p, A.p);
	d2f d2 = d2v_dot(c, c);
	d2f r2 = A.r + B.r;
	r2 = r2 * r2;
	return d2 < r2;
}

int d2circle_to_aabb(d2circle A, d2aabb B)
{
	c2v L = d2v_clamp(A.p, B.min, B.max);
	c2v ab = d2v_sub(A.p, L);
	d2f d2 = d2v_dot(ab, ab);
	d2f r2 = A.r * A.r;
	return d2 < r2;
}

int d2aabb_to_aabb(d2aabb A, d2aabb B)
{
	int d0 = B.max.x < A.min.x;
	int d1 = A.max.x < B.min.x;
	int d2 = B.max.y < A.min.y;
	int d3 = A.max.y < B.min.y;
	return !(d0 | d1 | d2 | d3);
}

int d2aabb_to_point(d2aabb A, d2v B)
{
	int d0 = B.x < A.min.x;
	int d1 = B.y < A.min.y;
	int d2 = B.x > A.max.x;
	int d3 = B.y > A.max.y;
	return !(d0 | d1 | d2 | d3);
}

int d2circle_to_point(d2circle A, d2v B)
{
	c2v n = d2v_sub(A.p, B);
	d2f d2 = d2v_dot(n, n);
	return d2 < A.r* A.r;
}

int d2circle_to_capsule(d2circle A, d2capsule B)
{
	return c2CircletoCapsule(A, B);
}

int d2aabb_to_capsule(d2aabb A, d2capsule B)
{
	if(c2GJK(&A, C2_TYPE_AABB, 0, &B, C2_TYPE_CAPSULE, 0, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

int d2capsule_to_capsule(d2capsule A, d2capsule B)
{
	if(c2GJK(&A, C2_TYPE_CAPSULE, 0, &B, C2_TYPE_CAPSULE, 0, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

int d2circle_to_poly(d2circle A, const d2poly* B, const d2t* bx)
{
	if(c2GJK(&A, C2_TYPE_CIRCLE, 0, B, C2_TYPE_POLY, bx, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

int d2aabb_to_poly(d2aabb A, const d2poly* B, const d2t* bx)
{
	if(c2GJK(&A, C2_TYPE_AABB, 0, B, C2_TYPE_POLY, bx, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

int d2capsule_to_poly(d2capsule A, const d2poly* B, const d2t* bx)
{
	if(c2GJK(&A, C2_TYPE_CAPSULE, 0, B, C2_TYPE_POLY, bx, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

int d2poly_to_poly(const d2poly* A, const d2t* ax, const d2poly* B, const d2t* bx)
{
	if(c2GJK(A, C2_TYPE_POLY, ax, B, C2_TYPE_POLY, bx, 0, 0, 1, 0, 0)) return 0;
	return 1;
}

// ray operations
// output is placed into the d2raycast struct, which represents the hit location
// of the ray. the out param contains no meaningful information if these funcs
// return 0
int d2ray_to_circle(d2ray A, d2circle B, d2raycast* out)
{
	return c2RaytoCircle(A, B, out);
}

int d2ray_to_aabb(d2ray A, d2aabb B, d2raycast* out)
{
	return c2RaytoAABB(A, B, out);
}

int d2ray_to_capsule(d2ray A, d2capsule B, d2raycast* out)
{
	return c2RaytoCapsule(A, B, out);
}

int d2ray_to_poly(d2ray A, const d2poly* B, const d2t* bx, d2raycast* out)
{
	return c2RaytoPoly(A, B, bx, out);
}

// manifold generation
// These functions are (generally) slower than the boolean versions, but will compute one
// or two points that represent the plane of contact. This information is usually needed
// to resolve and prevent shapes from colliding. If no collision occured the count member
// of the manifold struct is set to 0.
void d2circle_to_circle_manifold(d2circle A, d2circle B, d2manifold* m)
{
	c2CircletoCircleManifold(A, B, m);
}

void d2circle_to_aabb_manifold(d2circle A, d2aabb B, d2manifold* m)
{
	c2CircletoAABBManifold(A, B, m);
}

void d2circle_to_capsule_manifold(d2circle A, d2capsule B, d2manifold* m)
{
	c2CircletoCapsuleManifold(A, B, m);
}

void d2aabb_to_aabb_manifold(d2aabb A, d2aabb B, d2manifold* m)
{
	c2AABBtoAABBManifold(A, B, m);
}

void d2aabb_to_capsule_manifold(d2aabb A, d2capsule B, d2manifold* m)
{
	c2AABBtoCapsuleManifold(A, B, m);
}

void d2capsule_to_capsule_manifold(d2capsule A, d2capsule B, d2manifold* m)
{
	c2CapsuletoCapsuleManifold(A, B, m);
}

void d2circle_to_poly_manifold(d2circle A, const d2poly* B, const d2t* bx, d2manifold* m)
{
	c2CircletoPolyManifold(A, B, bx, m);
}

void d2aabb_to_poly_manifold(d2aabb A, const d2poly* B, const d2t* bx, d2manifold* m)
{
	c2AABBtoPolyManifold(A, B, bx, m);
}

void d2capsuleto_poly_manifold(d2capsule A, const d2poly* B, const d2t* bx, d2manifold* m)
{
	c2CapsuletoPolyManifold(A, B, bx, m);
}

void d2poly_to_poly_manifold(const d2poly* A, const d2t* ax, const d2poly* B, const d2t* bx, d2manifold* m)
{
	c2PolytoPolyManifold(A, ax, B, bx, m);
}
