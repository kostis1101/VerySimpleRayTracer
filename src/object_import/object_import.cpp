
#include "object_import.h"
#include "../green_math/green_math.h"
#include "../green_geometry/green_geometry.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>

using std::cout;
using std::endl;



typedef enum { VERTEX, FACE, UNSUPPORTED } dtype;

dtype get_dtype(FILE *file) {
	switch (getc(file)) {
		case 'v': {
			switch (getc(file)) {
				case ' ':
					return VERTEX;
				default:
					return UNSUPPORTED;
			}
		}
		case 'f': {
			switch (getc(file)) {
				case ' ':
					return FACE;
				default:
					return UNSUPPORTED;
			}
		}
	}
}

void skip_to_next(FILE* file, char c) {
	while (getc(file) != c);
}

std::vector<Triangle> read_obj_file(std::string filename) {
	// std::ifstream file(filename);
	FILE* file;
	fopen_s(&file, filename.c_str(), "r");

	if (!file) {
		std::cerr << "Could not open file" << std::endl;
		return {};
	}

	std::vector<Vec3f> verts;
	std::vector<Triangle> tris;

	dtype data_type;
	while (!feof(file)) {

		data_type = get_dtype(file);

		if (feof(file))
			break;

		switch (data_type) {
			case VERTEX: {
				Vec3f v;
				fscanf_s(file, "%f %f %f ", &v.x, &v.y, &v.z);
				verts.push_back(v);
				break;
			}
			case FACE: {
				long long i, j, k;
				fscanf_s(file, "%lld", &i);
				skip_to_next(file, ' ');
				fscanf_s(file, "%lld", &j);
				skip_to_next(file, ' ');
				fscanf_s(file, "%lld", &k);
				skip_to_next(file, '\n');
				tris.push_back(Triangle(verts[i - 1L], verts[j - 1L], verts[k - 1L]));
				break;
			}
			default:
				skip_to_next(file, '\n');
		}
	}

	fclose(file);

	return tris;
}