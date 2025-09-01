#pragma once

#include <cmath>
#include <iostream>
#include "vec_length.h"

using std::abs;
using std::sqrt;

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

	static T sqr_dist(Vec3<T>& v1, Vec3<T> v2) {
		return (v1 - v2).magn2();
	}

	static auto dist(Vec3<T>& v1, Vec3<T> v2) -> typename VecLength<T>::type {
		return (v1 - v2).magn();
	}

	static Vec3 max(Vec3& v1, Vec3& v2) {
		Vec3(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z));
	}

	static Vec3 min(Vec3& v1, Vec3& v2) {
		Vec3(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z));
	}

	static Vec3 all_ge(Vec3& v1, Vec3& v2) {
		return v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z;
	}

	static Vec3 all_lt(Vec3& v1, Vec3& v2) {
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