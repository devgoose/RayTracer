#include <math.h>
#include <vector>

#include "Scene.h"
#include "ViewWindow.h"

const float M_PI = 3.141592653;

float to_radians(float degrees) {
	return (degrees * M_PI) / 180.0;
}

ViewWindow::ViewWindow(const Scene& scene) {
	fillCorners(scene);
	fillLattice(scene);
}

Point3 ViewWindow::getPoint(int x, int y) {
	return view_lattice[x][y];
}

void ViewWindow::fillCorners(const Scene& scene) {
	Vector3 u = (scene.getViewdir().cross(scene.getUpdir())).toUnit();
	Vector3 v = (u.cross(scene.getViewdir())).toUnit();
	Vector3 n = (scene.getViewdir()).toUnit();

	float d = 2.0;

	float w = 2.0 * d * (float)tan(to_radians(scene.getHfov() / 2.0));
	float h = w / ((float)scene.getWidth() / (float)scene.getHeight());

	ul = scene.getEye() + (n * d) - (u * (w / 2.0)) + (v * (h / 2.0));
	ur = scene.getEye() + (n * d) + (u * (w / 2.0)) + (v * (h / 2.0));
	ll = scene.getEye() + (n * d) - (u * (w / 2.0)) - (v * (h / 2.0));
	lr = scene.getEye() + (n * d) + (u * (w / 2.0)) - (v * (h / 2.0));
}

void ViewWindow::fillLattice(const Scene& scene) {
	Vector3 h_offset = (ur - ul) / (2.0 * (float)scene.getWidth());
	Vector3 v_offset = (ll - ul) / (2.0 * (float)scene.getHeight());

	Vector3 h_delta = (ur - ul) / ((float)scene.getWidth());
	Vector3 v_delta = (ll - ul) / ((float)scene.getHeight());

	for (int i = 0; i < scene.getHeight(); i++) {
		std::vector<Point3> row;
		for (int j = 0; j < scene.getWidth(); j++) {
			row.push_back(ul + (h_delta * j) + (v_delta * i) + h_offset + v_offset);
		}
		view_lattice.push_back(row);
	}
}