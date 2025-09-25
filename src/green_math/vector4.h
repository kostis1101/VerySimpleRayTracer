#pragma once

#include <cmath>
#include <iostream>
#include "vec_length.h"

using std::abs;
using std::sqrt;

template<typename T = float>
struct Vec4 {
	T x, y, z, w;
	static const Vec4 zero;

	Vec4() : x(0), y(0), z(0), w(0) {}
	Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	T magn2() {
		return x * x + y * y + z * z + w * w;
	}

	auto magn() -> typename VecLength<T>::type {
		return std::sqrt(magn2());
	}

	void normalize() {
		T length = magn();
		x /= length;
		y /= length;
		z /= length;
		w /= length;
	}

	auto normalized() -> Vec4<typename VecLength<T>::type> {
		T length = magn();
		return Vec4(x / length, y / length, z / length, w / length);
	}

	Vec4<T> abs() {
		return Vec4(abs(x), abs(y), abs(z));
	}

	static T sqr_dist(Vec4<T>& v1, Vec4<T> v2) {
		return (v1 - v2).magn2();
	}

	static auto dist(Vec4<T>& v1, Vec4<T> v2) -> typename VecLength<T>::type {
		return (v1 - v2).magn();
	}

	static Vec4 max(Vec4& v1, Vec4& v2) {
		Vec4(max(v1.x, v2.x), max(v1.y, v2.y), max(v1.z, v2.z), max(v1.w, v2.w));
	}

	static Vec4 min(Vec4& v1, Vec4& v2) {
		Vec4(min(v1.x, v2.x), min(v1.y, v2.y), min(v1.z, v2.z), min(v1.w, v2.w));
	}

	static Vec4 all_ge(Vec4& v1, Vec4& v2) {
		return v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z && v1.w >= v2.w;
	}

	static Vec4 all_lt(Vec4& v1, Vec4& v2) {
		return v1.x < v2.x&& v1.y < v2.y&& v1.z < v2.z && v1.w < v2.w;
	}

	T operator[] (long n) {
		switch (n) {
		case 0:
			return x;
		case 1:
			return y;
		case 2:
			return z;
		case 3:
			return w;
		default:
			return 0;
		}
	}
};


template<typename T = float>
T dot(Vec4<T> v1, Vec4<T> v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

/* angle between two normalized vectors */
template<typename T = float>
float angle_between_norm(Vec4<T>& v1, Vec4<T>& v2) {
	return std::acos(dot(v1, v2));
}

template<typename T = float>
Vec4<T> operator+ (Vec4<T> v1, Vec4<T> v2) {
	return Vec4<T>(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
}

template<typename T = float>
Vec4<T> operator- (Vec4<T> v1, Vec4<T> v2) {
	return Vec4<T>(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
}

template<typename T = float>
Vec4<T> operator*(T x, Vec4<T> v) {
	return Vec4<T>(x * v.x, x * v.y, x * v.z, x * v.w);
}

template<typename T = float>
Vec4<T> operator*(Vec4<T> v, T x) {
	return Vec4<T>(x * v.x, x * v.y, x * v.z, x * v.w);
}

template<typename T = float>
Vec4<T> operator/(T x, Vec4<T> v) {
	return Vec4<T>(v.x / x, v.y / x, v.z / x, v.w / x);
}

template<typename T = float>
Vec4<T> operator/(Vec4<T> v, T x) {
	return Vec4<T>(v.x / x, v.y / x, v.z / x, v.w / x);
}

template<typename T = float>
std::ostream& operator<< (std::ostream& os, Vec4<T> v) {
	os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
	return os;
}

template <typename T = float>
Vec4<T> operator+(Vec4<T> v, T x) {
	return Vec4<T>(v.x + x, v.y + x, v.z + x, v.w + x);
}

template <typename T = float>
Vec4<T> operator-(Vec4<T> v, T x) {
	return Vec4<T>(v.x - x, v.y - x, v.z - x, v.w - x);
}

template<typename T>
const Vec4<T> Vec4<T>::zero = Vec4<T>();