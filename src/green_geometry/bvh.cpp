
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
	BoundBox3f bounds(tris[0].p1);
	Vec3f center;

	for (int i = 0; i < tris_len; i++) {
		bounds.insert_point(tris[i].p1);
		bounds.insert_point(tris[i].p2);
		bounds.insert_point(tris[i].p3);

		center = center + tris[i].p1 + tris[i].p2 + tris[i].p3;
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

	long long c1 = from_triangles(tris, i, nodes);
	long long c2 = from_triangles(tris + i, tris_len - i, nodes);
	nodes[newnode_index].children_indicies[0] = c1;
	nodes[newnode_index].children_indicies[1] = c2;

	//nodes[newnode_index].children_indicies[0] = from_triangles(tris, i, nodes);
	//cout << nodes[newnode_index].children_indicies[0] << endl;
	//nodes[newnode_index].children_indicies[1] = from_triangles(tris + i, tris_len - i, nodes);
	//cout << nodes[newnode_index].children_indicies[1] << endl;

	return newnode_index;
}
	
// slab method: en.wikipedia.org/wiki/Slab_method
bool BVH_node::ray_intersect(Rayf& r) {
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

	float tclose = max3(std::min(tmin[0], tmin[1]), std::min(tmin[1], tmax[1]), std::min(tmin[2], tmax[2]));
	float tfar = min3(std::max(tmin[0], tmin[1]), std::max(tmin[1], tmax[1]), std::max(tmin[2], tmax[2]));

	return 0 <= tclose && tclose <= tfar;
}

RayHitf BVH_node::search_ray_hit(Rayf& r, std::vector<BVH_node> &nodes) {
	if (!ray_intersect(r))
		return {};
	
	// children[0] == -1 if and only if the node is a leaf
	if (!children[0]) {
		float min_dist = std::numeric_limits<float>::infinity();
		RayHitf hit;
		for (int i = 0; i < tris_len; i++) {
			RayHitf h = tris[i].ray_intersect(r);
			if (h.hit && h.dist < min_dist) {
				min_dist = h.dist;
				hit = h;
			}
		}
		return hit;
	}

	// TODO: check which of the two children is in front and perform the first check there
	RayHitf h1 = children[0]->search_ray_hit(r, nodes);
	RayHitf h2 = children[1]->search_ray_hit(r, nodes);

	if (!h1.hit)
		return h2;
	else if (!h2.hit)
		return h1;
	else if (h1.dist < h2.dist)
		return h1;
	else return h2;
}


bool BVH::ray_intersect(Rayf& r) {
	return root->ray_intersect(r);
}

RayHitf BVH::search_ray_hit(Rayf& r) {
	return root->search_ray_hit(r, nodes);
}