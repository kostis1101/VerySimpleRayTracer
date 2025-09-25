#pragma once

#include "vector3.h"

template<typename T = float>
struct Ray {
	Vec3<T> origin;
	Vec3<T> direction;

	Ray() = default;
	Ray(Vec3<T> origin, Vec3<T> direction) : origin(origin), direction(direction.normalized()) {}

	Vec3<T> at(float t);
	Vec3<T> at(double t);
	Vec3<T> at(int t);
};

template <typename T>
Vec3<T> Ray<T>::at(float t) {
	return origin + direction * (T)t;
}

template <typename T>
Vec3<T> Ray<T>::at(double t) {
	return origin + direction * (T)t;
}

template <typename T>
Vec3<T> Ray<T>::at(int t) {
	return origin + direction * (T)t;
}