#pragma once

#include "../green_math/vector3.h"
#include "../green_math/ray_hit.h"
#include "../green_math/ray.h"

using Vec3f = Vec3<float>;
using Rayf = Ray<float>;
using RayHitf = RayHit<float>;

struct Vertex {
	Vec3f pos;
	Vec3f norm;

	Vertex() {}
	Vertex(Vec3f p) : pos(p) {}

	operator Vec3f() const {
		return pos;
	}
};

struct Triangle {
	Vertex *v1, *v2, *v3;
	Vec3f vn1, vn2, vn3;
	Vec3f normal;

	Triangle();
	// Triangle(Vec3f p1, Vec3f p2, Vec3f p3);
	// Triangle(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f normal);
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3);
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3, Vec3f normal);
	Triangle(Vertex* v1, Vertex* v2, Vertex* v3, Vec3f vn1, Vec3f vn2, Vec3f vn3, Vec3f normal);

	RayHitf ray_intersect(Rayf& r);

	/* with back face culling */
	RayHitf ray_intersect_bfc(Rayf& r);
	
	/* reorients the normal of the Triangle, so that its dot product with the dir vector is non-negative */
	void reorient_normal(Vec3f& dir);

	Vec3f center();
};