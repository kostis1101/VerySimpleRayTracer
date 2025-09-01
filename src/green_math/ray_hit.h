#pragma once

#include "vector3.h"

template <typename T>
struct RayHit {
	Vec3<T> point = Vec3<T>::zero;
	bool hit = false;

	RayHit() = default;
	RayHit(Vec3<T> point) : point(point), hit(true) {}
};