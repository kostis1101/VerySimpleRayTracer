
#include "green_geometry.h"

/* Ray */

Ray::Ray() : origin(Vec3::zero), direction(Vec3(0, 0, -1)) {}
Ray::Ray(Vec3 origin, Vec3 direction) : origin(origin), direction(direction.normalized()) {}

/* RayHit */

RayHit::RayHit() : point(Vec3::zero), hit(false) {}
RayHit::RayHit(Vec3 point) : point(point), hit(true) {}

/* Triangle */

Triangle::Triangle() : p1(), p2(), p3() {}
Triangle::Triangle(Vec3 p1, Vec3 p2, Vec3 p3) : p1(p1), p2(p2), p3(p3) {}

/* credit to wikipedia, because I was to bored to write it... */
RayHit Triangle::ray_intersect(Ray& r) {
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	Vec3 edge1 = p2 - p1;
	Vec3 edge2 = p3 - p1;
	Vec3 ray_cross_e2 = cross(r.direction, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return {};    // This ray is parallel to this triangle.

	float inv_det = 1.0 / det;
	Vec3 s = r.origin - p1;
	float u = inv_det * dot(s, ray_cross_e2);

	if ((u < 0 && abs(u) > epsilon) || (u > 1 && abs(u - 1) > epsilon))
		return {};

	Vec3 s_cross_e1 = cross(s, edge1);
	float v = inv_det * dot(r.direction, s_cross_e1);

	if ((v < 0 && abs(v) > epsilon) || (u + v > 1 && abs(u + v - 1) > epsilon))
		return {};

	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = inv_det * dot(edge2, s_cross_e1);

	if (t > epsilon) // ray intersection
	{
		return RayHit(Vec3(r.origin + r.direction * t));
	}
	else // This means that there is a line intersection but not a ray intersection.
		return {};
}

Vec3 Triangle::normal() {
	return cross(p3 - p1, p2 - p1).normalized();
}