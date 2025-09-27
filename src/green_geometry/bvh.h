#pragma once

#ifndef BVH_MAX_OBJ
#define BVH_MAX_OBJ 10
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
		long long children_indicies[2];
		BVH_node* children[2];
	};

	BVH_node(BoundBox3<float> boundbox, Triangle* tris, long tris_len)
		: boundbox(boundbox), tris(tris), tris_len(tris_len)
	{
		children_indicies[0] = -1;
		children_indicies[1] = -1;
	}

	/* returns the index of the root in the nodes vector */
	static long long from_triangles(Triangle* tris, long tris_len, std::vector<BVH_node> &nodes);

	bool ray_intersect(Rayf& r);

	RayHitf search_ray_hit(Rayf& r, std::vector<BVH_node> &nodes);

};

struct BVH {
	std::vector<BVH_node> nodes;
	BVH_node* root;

	BVH(std::vector<Triangle>& tris) {
		root = &nodes[BVH_node::from_triangles(&tris[0], tris.size(), nodes)];

		/* convert indicies to pointers */
		for (BVH_node& node : nodes) {
			long long i1 = node.children_indicies[0];
			long long i2 = node.children_indicies[1];

			node.children[0] = i1 != -1 ? &nodes[i1] : nullptr;
			node.children[1] = i2 != -1 ? &nodes[i2] : nullptr;
		}
	}

	bool ray_intersect(Rayf& r);

	RayHitf search_ray_hit(Rayf& r);

};