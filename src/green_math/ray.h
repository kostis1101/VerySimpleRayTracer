#pragma once

#include "vector3.h"

template<typename T>
struct Ray {
	Vec3<T> origin;
	Vec3<T> direction;

	Ray() = default;
	Ray(Vec3<T> origin, Vec3<T> direction) : origin(origin), direction(direction.normalized()) {}

	Vec3<T> at(float t);
	Vec3<T> at(double t);
	Vec3<T> at(int t);
};
