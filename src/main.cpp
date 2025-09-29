

#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>

#include "object_import/object_import.h"
#include "green_geometry/bvh.h"
#include "green_math/green_math.h"

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
	float cam_height = img.size[0] / (float)img.size[1];
	
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
				Vec3f n = hit_tri->normal;
				img.at<Vec3b>(j, i) = Vec3b((uchar)(255 * std::abs(n.x)), 255 * std::abs(n.y), 255 * std::abs(n.z));
			}
			else {
				img.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
			}
		}
	}
}


Vec3f shader(RayHitf& hit) {

	Vec3f n = hit.t2* hit.tri->vn2 + hit.t3 * hit.tri->vn3 + (1 - hit.t2 - hit.t3) * hit.tri->vn1;
	n.normalize();
	
	float t1 = dot(n, Vec3f(0.2, -0.5, 0.3).normalized());
	float t2 = dot(n, Vec3f(0.2, 0.2, 0.2).normalized());

	Vec3f c1(0.478539, 1.0, 0.415781);
	Vec3f c2(1.0, 0.246392, 1.0);

	return t1 * c1 * 0.5f + t2 * c2 * 0.5f;
}

void render(cv::Mat& img, Camera& cam, BVH& bvh) {
	float cam_width = 1;
	float cam_height = img.size[0] / (float)img.size[1];

	for (int j = 0; j < img.size[0]; j++) {
		for (int i = 0; i < img.size[1]; i++) {

			Rayf r = cam.get_ray_at_pixel(i, j);

			RayHitf hit = bvh.search_ray_hit(r);

			if (hit.hit) {
				Vec3f colour = shader(hit);
				colour.coords_clamp01();

				img.at<Vec3b>(j, i) = Vec3b(255 * colour.z, 255 * colour.y, 255 * colour.x);
			}
			else {
				img.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
			}
		}
	}
}


void render_region(cv::Mat& img, Camera& cam, BVH& bvh, int xmin, int xmax, int ymin, int ymax) {
	float cam_width = 1;
	float cam_height = img.size[0] / (float)img.size[1];

	for (int j = ymin; j < ymax; j++) {
		for (int i = xmin; i < xmax; i++) {

			Rayf r = cam.get_ray_at_pixel(i, j);

			RayHitf hit = bvh.search_ray_hit(r);

			if (hit.hit) {
				Vec3f colour = shader(hit);
				colour.coords_clamp01();

				img.at<Vec3b>(j, i) = Vec3b(255 * colour.z, 255 * colour.y, 255 * colour.x);
			}
			else {
				img.at<Vec3b>(j, i) = Vec3b(0, 0, 0);
			}
		}
	}
}

void render_threaded(cv::Mat& img, Camera& cam, BVH& bvh, int xsize, int ysize) {
	int x_threads = img.size[1] / xsize + (img.size[1] % xsize == 0 ? 0 : 1);
	int y_threads = img.size[0] / ysize + (img.size[0] % ysize == 0 ? 0 : 1);
	int num_threads = x_threads * y_threads;

	std::vector<std::thread> threads;
	threads.reserve(num_threads);
	for (int J = 0; J < y_threads; J++) {
		for (int I = 0; I < x_threads; I++) {
			threads.emplace_back(
				render_region,
				std::ref(img), std::ref(cam), std::ref(bvh),
				I * xsize, min((I + 1) * xsize, img.size[1]), J * ysize, min((J + 1) * ysize, img.size[0])
			);
		}
	}

	for (int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}
}


int main(int argc, char** argv)
{
	Mat img = cv::Mat(1000, 1000, CV_8UC3, cv::Vec3b(255, 0, 0));

	Camera cam = Camera(Vec3f(5, -5, 0), Vec3f(-1, 1, 0).normalized(), 0, 3, img.size[1], img.size[0]);
	Scene scene = read_obj_file(argv[1]);

	cout << "Triangles: " << scene.tris.size() << endl;

	auto t0 = std::chrono::high_resolution_clock::now();
	BVH bvh = BVH(scene.tris);
	auto t1 = std::chrono::high_resolution_clock::now();
	cout << "BVH created in: " << std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count() / 1000.f << "ms" << endl;
	render_threaded(img, cam, bvh, 250, 125);
	auto t2 = std::chrono::high_resolution_clock::now();
	cout << "Rendered in: " << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() / 1000.f << "ms" << endl;

	cv::imshow("window", img);
	cv::waitKey(0);

	return 0;
}