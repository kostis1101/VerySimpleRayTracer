

#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>

#include "green_math/green_math.h"
#include "green_geometry/green_geometry.h"
#include "object_import/object_import.h"

using namespace std;
using cv::Mat;
using cv::Vec3b;

struct Camera {
	Vec3f pos;
	Vec3f dir;
	float roll;
	Vec2i resolution;
	float focal_length;

	Camera(Vec3f pos, Vec3f dir, float roll, float focal_length, int resx, int resy) :
		pos(pos), dir(dir), roll(roll), resolution(resx, resy), focal_length(focal_length) {}

	Rayf get_ray_at_pixel(float i, float j) {
		Rayf r = Ray<float>(Vec3f::zero, Vec3f::zero);

		float hor_magn = std::sqrt(dir.x * dir.x + dir.y * dir.y);
		Vec3f vert = Vec3f(dir.z * dir.x / hor_magn, dir.z * dir.y / hor_magn, -hor_magn);
		Vec3f horz = cross(vert, dir);
	
		// Orthographic
		//r.origin = pos + 1 * ((j - resolution[1] / 2) * vert / (float)resolution[1] + (i - resolution[0] / 2) * horz / (float)resolution[0]);
		//r.direction = dir;

		// Perspective stuff
		r.direction = focal_length * dir + (j - resolution[0] / 2) * vert / (float)resolution[0] + (i - resolution[1] / 2) * horz / (float)resolution[1];
		r.direction.normalize();
		r.origin = pos;

		return r;
	}
};


void render(cv::Mat& img, Camera& cam, std::vector<Triangle>& tris) {
	float cam_width = 1;
	float cam_height = img.size[0] / img.size[1];
	
	for (int j = 0; j < img.size[0]; j++) {
		for (int i = 0; i < img.size[1]; i++) {
			//float x = (float)i / img.size[0] * cam_width - cam_width / 2;
			//float y = (float)j / img.size[1] * cam_height - cam_height / 2;

			Rayf r = cam.get_ray_at_pixel(i, j); // Ray(Vec3(x, y, 0), Vec3(0, 0, 1));

			float min_dist = std::numeric_limits<float>::infinity();
			RayHitf hit;
			Triangle* hit_tri = NULL;
			float d;
			for (Triangle& tri : tris) {
				RayHitf h = tri.ray_intersect(r);
				if (h.hit && (d = Vec3f::sqr_dist(h.point, cam.pos)) < min_dist) {
					min_dist = d;
					hit = h;
					hit_tri = &tri;
				}
			}

			//unsigned char colour = 255;
			//if (hit.hit) {
			//	colour = (unsigned char) (255.f * 2 / (1 + std::exp(-min_dist / 30)) - 1);
			//}
			//img.at<Vec3b>(j, i) = Vec3b(colour, colour, colour);

			if (hit.hit) {
				Vec3f n = hit_tri->normal();
				img.at<Vec3b>(j, i) = Vec3b((uchar)(255 * std::abs(n.x)), 255 * std::abs(n.y), 255 * std::abs(n.z));
			}
			else {
				img.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
			}
		}
	}
}



int main(int argc, char** argv)
{
	Mat img = cv::Mat(1000, 1000, CV_8UC3, cv::Vec3b(255, 0, 0));

	Triangle tri = Triangle(
		Vec3f(-0.5, 0, 1),
		Vec3f(0.2, 0.5, 1),
		Vec3f(0.2, -0.5, 1)
	);

	Camera cam = Camera(Vec3f(5, -5, 0), Vec3f(-1, 1, 0).normalized(), 0, 3, img.size[1], img.size[0]);
	std::vector<Triangle> tris = read_obj_file(argv[1]);

	cout << "Triangles: " << tris.size() << endl;

	auto t1 = std::chrono::high_resolution_clock::now();
	render(img, cam, tris);
	auto t2 = std::chrono::high_resolution_clock::now();

	cout << "Rendered in: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << endl;

	cv::imshow("window", img);
	cv::waitKey(0);

	return 0;
}