
#include "matrix4x4.h"


Matrix4x4::Matrix4x4(float _values[4][4]) {
	memcpy(values, _values, 16 * sizeof(float));
}

Vec4f Matrix4x4::tranform(Vec4f v) {
	return Vec4f(
		values[0][0] * v.x + values[0][1] * v.y + values[0][2] * v.z + values[0][3],
		values[1][0] * v.x + values[1][1] * v.y + values[1][2] * v.z + values[1][3],
		values[2][0] * v.x + values[2][1] * v.y + values[2][2] * v.z + values[2][3],
		values[3][0] + values[3][1] + values[3][2] + values[3][3]
	);
}

Vec3f Matrix4x4::tranform(Vec3f v) {
	return Vec3f(
		values[0][0] * v.x + values[0][1] * v.y + values[0][2] * v.z + values[0][3],
		values[1][0] * v.x + values[1][1] * v.y + values[1][2] * v.z + values[1][3],
		values[2][0] * v.x + values[2][1] * v.y + values[2][2] * v.z + values[2][3]
	);
}

Matrix4x4 dot(Matrix4x4* m1, Matrix4x4* m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.values[i][j] += m1->values[i][k] * m2->values[k][j];
			}
		}
	}

	return result;
}

Matrix4x4 dot(Matrix4x4 m1, Matrix4x4 m2) {
	Matrix4x4 result;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				result.values[i][j] += m1.values[i][k] * m2.values[k][j];
			}
		}
	}

	return result;
}

void Matrix4x4::scale(float s) {
	for (int i = 0; i < 16; i++) {
		*((float*)values + i) *= s;
	}
}

void Matrix4x4::offset(float s) {
	for (int i = 0; i < 16; i++) {
		*((float*)values + i) += s;
	}
}

Matrix4x4 Matrix4x4::operator+(Matrix4x4 m) {
	Matrix4x4 result;

	for (int i = 0; i < 16; i++) {
		*((float*)result.values + i) = *((float*)m.values + i) + *((float*)values + i);
	}

	return result;
}

Matrix4x4 Matrix4x4::operator-(Matrix4x4 m) {
	Matrix4x4 result;

	for (int i = 0; i < 16; i++) {
		*((float*)result.values + i) = *((float*)values + i) - *((float*)m.values + i);
	}

	return result;
}

static float id_array[][4] = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
const Matrix4x4 Matrix4x4::id = Matrix4x4(id_array);