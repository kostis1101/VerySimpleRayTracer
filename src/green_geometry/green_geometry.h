#pragma once

#include "../green_math/green_math.h"


struct Ray {
	Vec3 origin;
	Vec3 direction;

	Ray();
	Ray(Vec3 origin, Vec3 direction);
};

struct RayHit {
	Vec3 point;
	bool hit;

	RayHit();
	RayHit(Vec3 point);
};


struct Triangle {
	Vec3 p1, p2, p3;

	Triangle();
	Triangle(Vec3 p1, Vec3 p2, Vec3 p3);
	RayHit ray_intersect(Ray& r);
	Vec3 normal();
};