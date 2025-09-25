#pragma once

#ifndef BVH_MAX_OBJ
#define BVH_MAX_OBJ 10
#endif

#include "green_geometry.h"
#include "../green_math/bounds_box3.h"
#include <vector>

struct BVH {
	BoundBox3<float> boundbox;
	Triangle *tris;
	long tris_len;
	BVH* children[2];

	BVH(BoundBox3<float> boundbox, Triangle* tris, long tris_len, BVH* c1, BVH* c2)
		: boundbox(boundbox), tris(tris), tris_len(tris_len)
	{
		children[0] = c1;
		children[1] = c2;
	}

	static BVH *from_triangles(Triangle *tris, long tris_len);

	bool ray_intersect(Rayf& r);

	RayHitf search_ray_hit(Rayf& r);
};