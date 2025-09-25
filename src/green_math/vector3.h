#pragma once

#include <cmath>
#include <iostream>
#include "vec_length.h"

using std::abs;
using std::sqrt;

static float clamp01(float x) {
	if (x < 0) return 0;
	else if (x > 1) return 1;
	return x;
}

template<typename T = float>
struct Vec3 {
	T x, y, z;
	static const Vec3 zero;

	Vec3() : x(0), y(0), z(0) {}
	Vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	T magn2() {
		return x * x + y * y + z * z;
	}

	auto magn() -> typename VecLength<T>::type {
		return std::sqrt(magn2());
	}

	void normalize() {
		T length = magn();
		x /= length;
		y /= length;
		z /= length;
	}

	auto normalized() -> Vec3<typename VecLength<T>::type> {
		T length = magn();
		return Vec3(x / length, y / length, z / length);
	}
	
	Vec3<T> abs() {
		return Vec3(abs(x), abs(y), abs(z));
	}

	Vec3<float> rotate_around_axis(Vec3<float> axis, float angle) {
		axis.normalize();
		Vec3<float> v = this - axis * dot(axis, this);
		Vec3<float> vx = this - v;
		Vec3<float> vy = cross(vx, v);
		return v + vx * cos(angle) + vy * sin(angle);
	}

	void coords_clamp01() {
		x = clamp01(x);
		y = clamp01(y);
		z = clamp01(z);
	}

	static T sqr_dist(Vec3<T>& v1, Vec3<T> v2) {
		return (v1 - v2).magn2();
	}

	static auto dist(Vec3<T>& v1, Vec3<T> v2) -> typename VecLength<T>::type {
		return (v1 - v2).magn();
	}

	static Vec3 max(Vec3& v1, Vec3& v2) {
		return Vec3(std::max(v1.x, v2.x), std::max(v1.y, v2.y), std::max(v1.z, v2.z));
	}

	static Vec3 min(Vec3& v1, Vec3& v2) {
		return Vec3(std::min(v1.x, v2.x), std::min(v1.y, v2.y), std::min(v1.z, v2.z));
	}

	static bool all_ge(Vec3& v1, Vec3& v2) {
		return v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z;
	}

	static bool all_lt(Vec3& v1, Vec3& v2) {
		return v1.x < v2.x && v1.y < v2.y && v1.z < v2.z;
	}

	T operator[] (long n) {
		switch (n) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		default:
			return 0;
		}
	}

	int max_axis() {
		return ((x < z) & (y < z)) * 2 + ((x < y) & !(y < z));
	}
};


template<typename T = float>
T dot(Vec3<T> v1, Vec3<T> v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

template<typename T = float>
Vec3<T> cross(Vec3<T> v1, Vec3<T> v2) {
	return Vec3<T>(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

/* angle between two normalized vectors */
template<typename T = float>
float angle_between_norm(Vec3<T>& v1, Vec3<T>& v2) {
	return std::acos(dot(v1, v2));
}

template<typename T = float>
Vec3<T> operator+ (Vec3<T> v1, Vec3<T> v2) {
	return Vec3<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

template<typename T = float>
Vec3<T> operator- (Vec3<T> v1, Vec3<T> v2) {
	return Vec3<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

template<typename T = float>
Vec3<T> operator*(T x, Vec3<T> v) {
	return Vec3<T>(x * v.x, x * v.y, x * v.z);
}

template<typename T = float>
Vec3<T> operator*(Vec3<T> v, T x) {
	return Vec3<T>(x * v.x, x * v.y, x * v.z);
}

template<typename T = float>
Vec3<T> operator/(T x, Vec3<T> v) {
	return Vec3<T>(v.x / x, v.y / x, v.z / x);
}

template<typename T = float>
Vec3<T> operator/(Vec3<T> v, T x) {
	return Vec3<T>(v.x / x, v.y / x, v.z / x);
}

template<typename T = float>
std::ostream& operator<< (std::ostream& os, Vec3<T> v) {
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

template <typename T = float>
Vec3<T> operator+(Vec3<T> v, T x) {
	return Vec3<T>(v.x + x, v.y + x, v.z + x);
}

template <typename T = float>
Vec3<T> operator-(Vec3<T> v, T x) {
	return Vec3<T>(v.x - x, v.y - x, v.z - x);
}

template<typename T>
const Vec3<T> Vec3<T>::zero = Vec3<T>();

