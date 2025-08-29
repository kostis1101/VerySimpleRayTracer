
#include "green_math.h"
#include <iostream>
#include <ostream>

Vec3::Vec3() : x(0), y(0), z(0) {}

Vec3::Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vec3::distance(Vec3& v1, Vec3& v2) {
	return (v1 - v2).magn();
}

float Vec3::square_dist(Vec3& v1, Vec3& v2) {
	return (v1 - v2).magn2();
}

float Vec3::magn2() {
	return x * x + y * y + z * z;
}

float Vec3::magn() {
	return std::sqrt(x * x + y * y + z * z);
}

void Vec3::normalize() {
	float length = magn();
	x /= length;
	y /= length;
	z /= length;
}

Vec3 Vec3::normalized() {
	float m = magn();
	return Vec3(x / m, y / m, z / m);
}

const Vec3 Vec3::zero = Vec3(0, 0, 0);

float Vec3::dot(Vec3& v1, Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float dot(Vec3& v1, Vec3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vec3 Vec3::cross(Vec3& v1, Vec3& v2) {
	return Vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vec3 cross(Vec3& v1, Vec3& v2) {
	return Vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vec3 cross(Vec3 v1, Vec3 v2) {
	return Vec3(
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	);
}

Vec3 operator+ (Vec3 v1, Vec3 v2) {
	return Vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vec3 operator- (Vec3 v1, Vec3 v2) {
	return Vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vec3 operator*(float x, Vec3 v) {
	return Vec3(x * v.x, x * v.y, x * v.z);
}

Vec3 operator*(Vec3 v, float x) {
	return Vec3(x * v.x, x * v.y, x * v.z);
}

Vec3 operator/(float x, Vec3 v) {
	return Vec3(v.x / x, v.y / x, v.z / x);
}

Vec3 operator/(Vec3 v, float x) {
	return Vec3(v.x / x, v.y / x, v.z / x);
}

std::ostream& operator<< (std::ostream& os, Vec3 v) {
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}