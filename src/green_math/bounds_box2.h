#pragma once

#include <cmath>
#include "vector2.h"

template <typename T>
struct BoundBox2 {
	Vec2<T> min, max;

	BoundBox2(Vec2<T> v1, Vec2<T> v2) : min(Vec2<T>::min(v1, v2)), max(Vec2<T>::max(v1, v2)) {}
	BoundBox2(Vec2<T> p) : min(p), max(p) {}

	T volume();

	static BoundBox2 from_points(Vec2<T>* begin, long length);
};