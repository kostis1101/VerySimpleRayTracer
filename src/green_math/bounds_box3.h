#pragma once

#include <cmath>
#include "vector3.h"
#include "../green_geometry/green_geometry.h"

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

	Vec3<T> center() {
		return (max + min) / 2.f;
	}

	static BoundBox3<T> Union(BoundBox3<T>& b, Vec3<T> p);
	static BoundBox3<T> Intersection(BoundBox3<T>& b1, BoundBox3<T>& b2);
	static bool overlap(BoundBox3<T>& b1, BoundBox3<T>& b2);
	static BoundBox3 from_points(Vec3<T>* begin, long length);
	static BoundBox3 from_tris(Triangle* begin, long length);
};


/* IMPLEMENTATIONS */

using std::min;
using std::max;
using std::abs;

template <typename T>
T BoundBox3<T>::volume() {
	return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

template <typename T>
BoundBox3<T> BoundBox3<T>::from_points(Vec3<T>* begin, long length) {
	Vec3<T> min = begin[0];
	Vec3<T> max = begin[0];

	for (int i = 1; i < length; i++) {
		min = Vec3<T>::min(min, begin[i]);
		max = Vec3<T>::max(max, begin[i]);
	}

	return BoundBox3(min, max);
}

template <typename T>
BoundBox3<T> BoundBox3<T>::from_tris(Triangle* begin, long length) {
	Vec3<T> min = begin[0].p1;
	Vec3<T> max = begin[0].p1;

	for (int i = 0; i < length; i++) {
		min = Vec3<T>::min(min, begin[i].p1);
		min = Vec3<T>::min(min, begin[i].p2);
		min = Vec3<T>::min(min, begin[i].p3);

		max = Vec3<T>::max(max, begin[i].p1);
		max = Vec3<T>::max(max, begin[i].p2);
		max = Vec3<T>::max(max, begin[i].p3);
	}

	return BoundBox3(min, max);
}

template <typename T>
void BoundBox3<T>::insert_point(Vec3<T>& p) {
	min = Vec3<T>::min(min, p);
	max = Vec3<T>::max(max, p);
}

template <typename T>
BoundBox3<T> BoundBox3<T>::Union(BoundBox3<T>& b, Vec3<T> p) {
	return BoundBox3(Vec3<T>::min(b.min, p), Vec3<T>::max(p, b.max));
}

template <typename T>
BoundBox3<T> BoundBox3<T>::Intersection(BoundBox3<T>& b1, BoundBox3<T>& b2) {
	return BoundBox3(Vec3<T>::max(b1.min, b2.min), Vec3<T>::min(b1.max, b2.max));
}

template <typename T>
bool BoundBox3<T>::overlap(BoundBox3<T>& b1, BoundBox3<T>& b2) {
	return Vec3<T>::all_ge(b1.max, b2.min) && Vec3<T>::all_ge(b2.max, b1.min);
}

template <typename T>
bool BoundBox3<T>::is_inside(Vec3<T> p) {
	return Vec3<T>::all_ge(p, min) && Vec3<T>::all_ge(max, p);
}

template <typename T>
bool BoundBox3<T>::is_strict_inside(Vec3<T> p) {
	return Vec3<T>::all_lt(min, p) && Vec3<T>::all_lt(p, max);
}

template <typename T>
template <typename U>
decltype(T() - U()) BoundBox3<T>::distance2(Vec3<U> p) {
	decltype(T() - U()) x = min(abs(p.x - min.x), abs(p.x - max.x));
	decltype(T() - U()) y = min(abs(p.y - min.y), abs(p.y - max.y));
	decltype(T() - U()) z = min(abs(p.z - min.z), abs(p.z - max.z));
	return x * x + y * y + z * z;
}

template <typename T>
template <typename U>
VecLength<decltype (T() - U())> BoundBox3<T>::distance(Vec3<U> p) {
	return sqrt(distance2(p));
}

template <typename T, typename U>
BoundBox3<decltype(T() - U())> expand(BoundBox3<T>& b, U amount) {
	return BoundBox3<decltype(T() - U())>(b.min + amount, b.max + amount);
}
