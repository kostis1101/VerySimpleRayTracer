#pragma once

#include "vector3.h"

template <typename T>
struct RayHit {
	Vec3<T> point = Vec3<T>::zero;
	bool hit = false;
	float dist = std::numeric_limits<float>::infinity();
	struct Triangle* tri = nullptr;

	RayHit() = default;
	RayHit(Vec3<T> point) : point(point), hit(true) {}
	RayHit(Vec3<T> point, float dist) : point(point), hit(true), dist(dist) {}
	RayHit(Vec3<T> point, float dist, Triangle* tri) : point(point), hit(true), dist(dist), tri(tri) {}
};
