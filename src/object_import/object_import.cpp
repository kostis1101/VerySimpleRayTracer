
#include "object_import.h"
#include "../green_math/green_math.h"
#include "../green_geometry/green_geometry.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include "../green_math/green_math.h"

using std::cout;
using std::endl;



typedef enum { VERTEX, FACE, VERTEX_NORMAL, UNSUPPORTED } dtype;

dtype get_dtype(FILE *file) {
	switch (getc(file)) {
		case 'v': {
			switch (getc(file)) {
				case ' ':
					return VERTEX;
				case 'n':
					return VERTEX_NORMAL;
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


/* supposes that the vertex normals and tagents are given */

Scene read_obj_file(std::string filename) {
	// std::ifstream file(filename);
	FILE* file;
	fopen_s(&file, filename.c_str(), "r");

	if (!file) {
		std::cerr << "Could not open file" << std::endl;
		return {};
	}

	Scene scn;

	std::vector<Vec3f> vert_norm;

	dtype data_type;
	while (!feof(file)) {

		data_type = get_dtype(file);

		if (feof(file))
			break;
		
		switch (data_type) {
			case VERTEX: {
				Vertex v;
				fscanf_s(file, "%f %f %f ", &v.pos.x, &v.pos.y, &v.pos.z);
				scn.verts.push_back(v);
				break;
			}
			case VERTEX_NORMAL: {
				Vec3f vn;
				fscanf_s(file, "%f %f %f ", &vn.x, &vn.y, &vn.z);
				vert_norm.push_back(vn);
				break;
			}
			case FACE: {
				long long i, it, in;
				long long j, jt, jn;
				long long k, kt, kn;

				fscanf_s(file, "%lld/%lld/%lld %lld/%lld/%lld %lld/%lld/%lld ", &i, &it, &in, &j, &jt, &jn, &k, &kt, &kn);
				
				Vec3f tn = cross(scn.verts[j - 1L].pos - scn.verts[i - 1L].pos, scn.verts[k - 1L].pos - scn.verts[i - 1L].pos);
				if (dot(tn, vert_norm[in - 1]) < 0)
					tn = tn * -1.f;

				scn.tris.emplace_back(&scn.verts[i - 1L], &scn.verts[j - 1L], &scn.verts[k - 1L],
					vert_norm[in - 1], vert_norm[jn - 1], vert_norm[kn - 1], tn);
				break;
			}
			default:
				skip_to_next(file, '\n');
		}
	}

	fclose(file);

	cout << &scn.tris[0] << endl;

	return scn;
}
