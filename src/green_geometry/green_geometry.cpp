
#include "green_geometry.h"
#include "../green_math/green_math.h"


Triangle::Triangle() : p1(), p2(), p3() {}
Triangle::Triangle(Vec3f p1, Vec3f p2, Vec3f p3) : p1(p1), p2(p2), p3(p3) {}

/* credit to wikipedia, because I was to bored to write it... */
RayHitf Triangle::ray_intersect(Rayf& r) {
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	Vec3f edge1 = p2 - p1;
	Vec3f edge2 = p3 - p1;
	Vec3f ray_cross_e2 = cross(r.direction, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return {};    // This ray is parallel to this triangle.

	float inv_det = 1.0f / det;
	Vec3f s = r.origin - p1;
	float u = inv_det * dot(s, ray_cross_e2);

	if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon))
		return {};

	Vec3f s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(r.direction, s_cross_e1);

	if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
		return {};

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = inv_det * dot(edge2, s_cross_e1);

	if (t > epsilon) // ray intersection
	{
		return RayHitf(Vec3f(r.origin + r.direction * t), t, this);
	}
	else // This means that there is a line intersection but not a ray intersection.
		return {};
}

Vec3f Triangle::normal() {
	return cross(p3 - p1, p2 - p1).normalized();
}

Vec3f Triangle::center() {
	return (p1 + p2 + p3) / 3.f;
}