#pragma once

#include <cmath>
#include <iostream>
#include "vec_length.h"

using std::abs;
using std::sqrt;

template<typename T = float>
struct Vec2 {
	T x, y;
	static const Vec2 zero;

	Vec2() : x(0), y(0) {}
	Vec2(T x, T y) : x(x), y(y) {}

	T magn2() {
		return x * x + y * y;
	}

	typename VecLength<T>::type magn() {
		return std::sqrt(magn2());
	}

	void normalize() {
		T length = magn();
		x /= length;
		y /= length;
	}

	auto normalized() -> Vec2<typename VecLength<T>::type> {
		T length = magn();
		return Vec2(x / length, y / length);
	}

	Vec2<T> abs() {
		return Vec2(abs(x), abs(y));
	}

	static VecLength<T> sqr_dist(Vec2<T>& v1, Vec2<T> v2) {
		return (v1 - v2).magn2();
	}

	static VecLength<T> dist(Vec2<T>& v1, Vec2<T> v2) {
		return (v1 - v2).magn();
	}

	static Vec2 max(Vec2& v1, Vec2& v2) {
		Vec2(max(v1.x, v2.x), max(v1.y, v2.y));
	}

	static Vec2 min(Vec2& v1, Vec2& v2) {
		Vec2(min(v1.x, v2.x), min(v1.y, v2.y));
	}

	T operator[] (long n) {
		switch (n) {
		case 0:
			return x;
		case 1:
			return y;
		default:
			return 0;
		}
	}
};


template<typename T = float>
Vec2<T> dot(Vec2<T> v1, Vec2<T> v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

template<typename T = float>
T cross(Vec2<T> v1, Vec2<T> v2) {
	return v1.x * v2.y - v1.y * v2.x;
}

/* angle between two normalized vectors */
template<typename T = float>
float angle_between_norm(Vec2<T>& v1, Vec2<T>& v2) {
	return std::acos(dot(v1, v2));
}

template<typename T = float>
Vec2<T> operator+ (Vec2<T>& v1, Vec2<T>& v2) {
	return Vec2<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template<typename T = float>
Vec2<T> operator- (Vec2<T>& v1, Vec2<T>& v2) {
	return Vec2<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template<typename T = float>
Vec2<T> operator*(float x, Vec2<T>& v) {
	return Vec2<T>(x * v.x, x * v.y, x * v.z);
}

template<typename T = float>
Vec2<T> operator*(Vec2<T>& v, float x) {
	return Vec2<T>(x * v.x, x * v.y, x * v.z);
}

template<typename T = float>
Vec2<T> operator/(float x, Vec2<T>& v) {
	return Vec2<T>(v.x / x, v.y / x, v.z / x);
}

template<typename T = float>
Vec2<T> operator/(Vec2<T>& v, float x) {
	return Vec2<T>(v.x / x, v.y / x, v.z / x);
}

template<typename T = float>
std::ostream& operator<< (std::ostream& os, Vec2<T>& v) {
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

template<typename T>
const Vec2<T> Vec2<T>::zero = Vec2<T>(0, 0);