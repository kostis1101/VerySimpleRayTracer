#pragma once

#include "vector3.h"

template <typename T>
struct RayHit {
	Vec3<T> point = Vec3<T>::zero;
	bool hit = false;
	float dist = std::numeric_limits<float>::infinity();
	struct Triangle* tri = nullptr;
	float t2, t3;

	RayHit() = default;
	RayHit(Vec3<T> point) : point(point), hit(true) {}
	RayHit(Vec3<T> point, float dist) : point(point), hit(true), dist(dist) {}
	RayHit(Vec3<T> point, float dist, Triangle* tri) : point(point), hit(true), dist(dist), tri(tri) {}
	RayHit(Vec3<T> point, float dist, Triangle* tri, float t2, float t3) :
		point(point), hit(true), dist(dist), tri(tri), t2(t2), t3(t3) {}
};
