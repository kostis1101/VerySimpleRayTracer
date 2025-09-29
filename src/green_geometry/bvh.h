#pragma once

#ifndef BVH_MAX_OBJ
#define BVH_MAX_OBJ 20
#endif

#include "green_geometry.h"
#include "../green_math/bounds_box3.h"
#include <vector>
#include <iostream>

using std::cout;
using std::endl;



struct BVH_node {
	BoundBox3<float> boundbox;
	Triangle* tris;
	long tris_len;
	
	/* when creating the tree, we use the indicies. Then, at the end, we change it to the pointer, since it's about 5% faster */
	union {
		long long sec_child_index;
		BVH_node* sec_child;
	};

	BVH_node(BoundBox3<float> boundbox, Triangle* tris, long tris_len)
		: boundbox(boundbox), tris(tris), tris_len(tris_len)
	{
		sec_child_index = -1;
	}

	/* returns the index of the root in the nodes vector */
	static long long from_triangles(Triangle* tris, long tris_len, std::vector<BVH_node> &nodes);

	float ray_intersect(Rayf& r);

	RayHitf search_ray_hit(Rayf& r, std::vector<BVH_node> &nodes, float min_dist = INFINITY);

};

struct BVH {
	std::vector<BVH_node> nodes;
	BVH_node* root;

	BVH(std::vector<Triangle>& tris) {
		root = &nodes[BVH_node::from_triangles(&tris[0], tris.size(), nodes)];

		/* convert indicies to pointers */
		for (BVH_node& node : nodes) {
			long long i = node.sec_child_index;

			node.sec_child = i != -1 ? &nodes[i] : nullptr;
		}
	}

	bool ray_intersect(Rayf& r);

	RayHitf search_ray_hit(Rayf& r);

};