#pragma once

#include "ray.h"

template <typename T>
Vec3<T> Ray<T>::at(float t) {
	return origin + direction * t;
}

template <typename T>
Vec3<T> Ray<T>::at(double t) {
	return origin + direction * t;
}

template <typename T>
Vec3<T> Ray<T>::at(int t) {
	return origin + direction * t;
}