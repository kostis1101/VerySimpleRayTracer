#pragma once
#include <vector>
#include <string>
#include "../green_geometry/green_geometry.h"

using std::vector;

struct Scene {
	vector<Vertex> verts;
	vector<Triangle> tris;
};


Scene read_obj_file(std::string filename);