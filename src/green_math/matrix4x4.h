
#pragma once

#include "green_math.h"

struct Matrix4x4 {
	float values[4][4];
	static const Matrix4x4 id;

	Matrix4x4() = default;
	Matrix4x4(float[4][4]);

	Vec4f tranform(Vec4f v);
	Vec3f tranform(Vec3f v);

	Matrix4x4 operator+(Matrix4x4 m);
	Matrix4x4 operator-(Matrix4x4 m);

	void scale(float);
	void offset(float);

	static Matrix4x4 Translation(Vec3f v) {
		Matrix4x4 result = id;
		result.values[0][3] = v[0];
		result.values[1][3] = v[1];
		result.values[2][3] = v[2];
		return result;
	}
};

Matrix4x4 dot(Matrix4x4* m1, Matrix4x4* m2);

Matrix4x4 dot(Matrix4x4 m1, Matrix4x4 m2);

