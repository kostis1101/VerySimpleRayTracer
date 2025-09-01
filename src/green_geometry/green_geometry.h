#pragma once

#include "../green_math/green_math.h"


struct Triangle {
	Vec3f p1, p2, p3;

	Triangle();
	Triangle(Vec3f p1, Vec3f p2, Vec3f p3);
	RayHitf ray_intersect(Rayf& r);
	Vec3f normal();
};