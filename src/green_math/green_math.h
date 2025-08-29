
#pragma once
#include <ostream>

struct Vec3 {
	float x;
	float y;
	float z;
	static const Vec3 zero;

	Vec3();
	Vec3(float x, float y, float z);

	float magn();
	float magn2();
	void normalize();
	Vec3 normalized();

	static float dot(Vec3&, Vec3&);
	static Vec3 cross(Vec3&, Vec3&);
	static float distance(Vec3&, Vec3&);
	static float square_dist(Vec3&, Vec3&);
};

float dot(Vec3& v1, Vec3& v2);

Vec3 cross(Vec3 v1, Vec3 v2);

Vec3 operator+ (Vec3 v1, Vec3 v2);

Vec3 operator- (Vec3 v1, Vec3 v2);

Vec3 operator*(float x, Vec3 v);

Vec3 operator*(Vec3 v, float x);

Vec3 operator/(float x, Vec3 v);

Vec3 operator/(Vec3 v, float x);

std::ostream& operator<< (std::ostream& os, Vec3 v);