
#include "bvh.h"


#include <iostream>


using std::cout;
using std::endl;

typedef BoundBox3<float> BoundBox3f;

void swap(Triangle *t1, Triangle* t2) {
	Triangle temp = *t1;
	*t1 = *t2;
	*t2 = temp;
}


static float min3(float a, float b, float c) {
	if (a < b && a < c)
		return a;
	else if (b < c)
		return b;
	return c;
}


static float max3(float a, float b, float c) {
	if (a > b && a > c)
		return a;
	else if (b > c)
		return b;
	return c;
}

typedef std::pair<BoundBox3f, Vec3f> tri_info;

/* returns the boundingbox and the center of centers of the triangles */
tri_info get_tri_info(Triangle* tris, long tris_len) {
	BoundBox3f bounds(tris[0].v1->pos);
	Vec3f center;

	for (int i = 0; i < tris_len; i++) {
		bounds.insert_point(tris[i].v1->pos);
		bounds.insert_point(tris[i].v2->pos);
		bounds.insert_point(tris[i].v3->pos);

		center = center + tris[i].v1->pos + tris[i].v2->pos + tris[i].v3->pos;
	}

	return { bounds, center / (3.f * (float)tris_len) };
}


/* TODO: optimize cache locality */
long long BVH_node::from_triangles(Triangle* tris, long tris_len, std::vector<BVH_node>& nodes) {

	tri_info ti = get_tri_info(tris, tris_len);
	BoundBox3f boundbox = ti.first;
	Vec3<float> center = ti.second;

	if (tris_len <= BVH_MAX_OBJ) {
		nodes.emplace_back(boundbox, tris, tris_len);
		return nodes.size() - 1;
	}

	Vec3f diag = boundbox.diag();

	int dim = diag.max_axis();

	/* move triangles */
	int i = 0;
	int j = tris_len - 1;

	// TODO: precompute bounding boxes of children

	for (; tris[i].center()[dim] <= center[dim]; i++);
	for (; tris[j].center()[dim] > center[dim]; j--);


	while (i < j) {
		swap(tris + i, tris + j);

		for (; tris[i].center()[dim] <= center[dim]; i++);
		for (; tris[j].center()[dim] > center[dim]; j--);
	}

	unsigned int newnode_index = nodes.size();
	nodes.emplace_back(boundbox, tris, tris_len);

	from_triangles(tris, i, nodes);
	long long c2 = from_triangles(tris + i, tris_len - i, nodes);
	nodes[newnode_index].sec_child_index = c2;

	return newnode_index;
}
	
// slab method: en.wikipedia.org/wiki/Slab_method
// returns the distance from the intersection. If there is no intersection, returns INFINITY
float BVH_node::ray_intersect(Rayf& r) {
	float tmin[3] = {
		(boundbox.min.x - r.origin.x) / r.direction.x,
		(boundbox.min.y - r.origin.y) / r.direction.y,
		(boundbox.min.z - r.origin.z) / r.direction.z,
	};

	float tmax[3] = {
		(boundbox.max.x - r.origin.x) / r.direction.x,
		(boundbox.max.y - r.origin.y) / r.direction.y,
		(boundbox.max.z - r.origin.z) / r.direction.z,
	};

	float tclose = max3(std::min(tmin[0], tmax[0]), std::min(tmin[1], tmax[1]), std::min(tmin[2], tmax[2]));
	float tfar = min3(std::max(tmin[0], tmax[0]), std::max(tmin[1], tmax[1]), std::max(tmin[2], tmax[2]));

	if (0 <= tclose && tclose <= tfar)
		return tclose;
	return INFINITY;
}

RayHitf BVH_node::search_ray_hit(Rayf& r, std::vector<BVH_node> &nodes, float min_dist) {

	//cout << r.at(ray_intersect(r)) << endl;
	//cout << "[" << this->boundbox.min << ", " << this->boundbox.max << "]," << endl;

	if (!sec_child) {
		float min_dist = std::numeric_limits<float>::infinity();
		RayHitf hit;
		for (int i = 0; i < tris_len; i++) {
			RayHitf h = tris[i].ray_intersect_bfc(r);
			if (h.hit && h.dist < min_dist) {
				min_dist = h.dist;
				hit = h;
			}
		}
		return hit;
	}

	// TODO: check which of the two children is in front and perform the first check there

	float d1 = (this + 1)->ray_intersect(r);
	float d2 = sec_child->ray_intersect(r);

/*	if (min_dist <= d1 && min_dist <= d2)
		return {};
	else if (min_dist <= d2)
		return (this + 1)->search_ray_hit(r, nodes, min_dist);
	else if (min_dist <= d1)
		return sec_child->search_ray_hit(r, nodes, min_dist);
	else if (d1 < d2) {
		RayHitf h1 = (this + 1)->search_ray_hit(r, nodes, min_dist);
		RayHitf h2;
		min_dist = min(h1.dist, min_dist);
		if (d2 < min_dist)
			h2 = sec_child->search_ray_hit(r, nodes, min_dist);
		if (h2.dist < h1.dist)
			return h2;
		return h1;
	}
	else {
		RayHitf h1;
		RayHitf h2 = sec_child->search_ray_hit(r, nodes, min_dist);
		min_dist = min(h2.dist, min_dist);
		if (d1 < min_dist)
			h1 = (this + 1)->search_ray_hit(r, nodes, min_dist);
		if (h2.dist < h1.dist)
			return h2;
		return h1;
	}*/

	//////////////////////////////////////////////////

/*	BVH_node* child_close = this + 1;
	BVH_node* child_far = sec_child;
	float d_close = d1;
	float d_far = d2;

	if (d2 < d1) {
		child_close = sec_child;
		child_far = this + 1;

		d_close = d2;
		d_far = d1;
	}

	if (min_dist <= d_close)
		return {};
	h1 = child_close->search_ray_hit(r, nodes, min_dist);
	min_dist = min(min_dist, h1.dist);
	
	if (min_dist <= d_far)
		return h1;
	h2 = child_far->search_ray_hit(r, nodes, min_dist);

	if (h2.dist < h1.dist)
		return h2;
	return h1; */

	//////////////////////////////////////////////////

	RayHitf h1, h2;

	if (min_dist <= d1 && min_dist <= d2)
		return {};
	else if (d1 < d2) {
		h1 = (this + 1)->search_ray_hit(r, nodes, min_dist);
		min_dist = min(h1.dist, min_dist);
		if (d2 < min_dist)
			h2 = sec_child->search_ray_hit(r, nodes, min_dist);
	}
	else {
		h2 = sec_child->search_ray_hit(r, nodes, min_dist);
		min_dist = min(h2.dist, min_dist);
		if (d1 < min_dist)
			h1 = (this + 1)->search_ray_hit(r, nodes, min_dist);
	}

	if (h2.dist < h1.dist)
		return h2;
	return h1;

}


bool BVH::ray_intersect(Rayf& r) {
	return root->ray_intersect(r);
}

RayHitf BVH::search_ray_hit(Rayf& r) {
	return root->search_ray_hit(r, nodes);
}