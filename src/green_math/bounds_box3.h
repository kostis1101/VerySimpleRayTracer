#pragma once

#include <cmath>
#include "vector3.h"

template <typename T>
struct BoundBox3 {
	Vec3<T> min, max;

	BoundBox3(Vec3<T> v1, Vec3<T> v2) : min(Vec3<T>::min(v1, v2)), max(Vec3<T>::max(v1, v2)) {}
	BoundBox3(Vec3<T> p) : min(p), max(p) {}

	T volume();

	void insert_point(Vec3<T>& p);
	bool is_inside(Vec3<T> p);
	bool is_strict_inside(Vec3<T> p);

	/* returns the squared distance from p to the closest point to p inside the bounding box */
	template <typename U>
	decltype(T() - U()) distance2(Vec3<U> p);

	/* return the distance from p to the dlosest point to p inside the bounding box*/
	template <typename U>
	VecLength<decltype (T() - U())> distance(Vec3<U> p);

	Vec3<T> diag() {
		return max - min;
	}

	static BoundBox3<T> Union(BoundBox3<T>& b, Vec3<T> p);
	static BoundBox3<T> Intersection(BoundBox3<T>& b1, BoundBox3<T>& b2);
	static bool overlap(BoundBox3<T>& b1, BoundBox3<T>& b2);
	static BoundBox3 from_points(Vec3<T>* begin, long length);
};

template <typename T, typename U>
BoundBox3<decltype(T() - U())> expand(BoundBox3<T>& b, U amount);