
#include "green_geometry.h"
#include "../green_math/green_math.h"


Triangle::Triangle() : v1(), v2(), v3() {}
/*Triangle::Triangle(Vec3f p1, Vec3f p2, Vec3f p3) {
	v1 = new Vertex(p1);
	v2 = new Vertex(p2);
	v3 = new Vertex(p3);

	normal = cross(p2 - p1, p3 - p1);
	normal.normalize();
}
Triangle::Triangle(Vec3f p1, Vec3f p2, Vec3f p3, Vec3f normal) : normal(normal) {
	v1 = new Vertex(p1);
	v2 = new Vertex(p2);
	v3 = new Vertex(p3);
}*/

Triangle::Triangle(Vertex *p1, Vertex* p2, Vertex* p3) : v1(p1), v2(p2), v3(p3) {
	normal = cross(p2->pos - p1->pos, p3->pos - p1->pos);
	normal.normalize();
}

Triangle::Triangle(Vertex* p1, Vertex* p2, Vertex* p3, Vec3f normal) : v1(p1), v2(p2), v3(p3), normal(normal) {}
Triangle::Triangle(Vertex* p1, Vertex* p2, Vertex* p3, Vec3f vn1, Vec3f vn2, Vec3f vn3, Vec3f normal)
	: v1(p1), v2(p2), v3(p3), vn1(vn1), vn2(vn2), vn3(vn3), normal(normal) {}


/* credit to wikipedia, because I was to bored to write it... */
RayHitf Triangle::ray_intersect(Rayf& r) {
	constexpr float epsilon = std::numeric_limits<float>::epsilon();

	Vec3f edge1 = v2->pos - v1->pos;
	Vec3f edge2 = v3->pos - v1->pos;
	Vec3f ray_cross_e2 = cross(r.direction, edge2);
	float det = dot(edge1, ray_cross_e2);

	if (det > -epsilon && det < epsilon)
		return {};    // This ray is parallel to this triangle.

	float inv_det = 1.0f / det;
	Vec3f s = r.origin - v1->pos;
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
		return RayHitf(Vec3f(r.origin + r.direction * t), t, this, u, v);
	}
	else // This means that there is a line intersection but not a ray intersection.
		return {};
}


RayHitf Triangle::ray_intersect_bfc(Rayf& r) {
	if (dot(r.direction, normal) > 0)
		return {};
	return ray_intersect(r);
}

Vec3f Triangle::center() {
	return (v1->pos + v2->pos + v3->pos) / 3.f;
}

void Triangle::reorient_normal(Vec3f& dir) {
	if (dot(normal, dir) < 0) {
		normal.x = -normal.x;
		normal.y = -normal.y;
		normal.z = -normal.z;
	}
}