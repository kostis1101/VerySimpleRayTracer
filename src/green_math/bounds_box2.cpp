
#include "bounds_box2.h"

template <typename T>
T BoundBox2<T>::volume() {
	return (max.x - min.x) * (max.y - min.y) * (max.z - min.z);
}

template <typename T>
static BoundBox2<T> BoundBox2<T>::from_points(Vec2<T>* begin, long length) {
	Vec2<T> min = begin[0];
	Vec2<T> max = begin[0];

	for (int i = 1; i < length; i++) {
		min = Vec2<T>::min(min, begin[i]);
		max = Vec2<T>::max(max, begin[i]);
	}

	return BoundBox2(min, max);
}
