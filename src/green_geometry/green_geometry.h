#pragma once

#include "../green_math/vector3.h"
#include "../green_math/ray_hit.h"
#include "../green_math/ray.h"

using Vec3f = Vec3<float>;
using Rayf = Ray<float>;
using RayHitf = RayHit<float>;

struct Triangle {
	Vec3f p1, p2, p3;

	Triangle();
	Triangle(Vec3f p1, Vec3f p2, Vec3f p3);
	RayHitf ray_intersect(Rayf& r);
	Vec3f normal();

	Vec3f center();
};