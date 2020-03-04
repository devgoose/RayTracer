#include <math.h>
#include <algorithm>

#include "Sphere.h"

Sphere::Sphere(const Sphere& sphere) {
	radius = sphere.radius;
	mtlcolor = sphere.mtlcolor;
	pos = sphere.pos;
}

bool Sphere::Intersect(const Ray& ray, Point3* intersection, const Scene& scene)  {
	float rx = ray.getPos().getX();			// Ray coords
	float ry = ray.getPos().getY();
	float rz = ray.getPos().getZ();
	float dx = ray.getDirection().getX();	// Ray dir vals
	float dy = ray.getDirection().getY();
	float dz = ray.getDirection().getZ();
	float sx = this->getPos().getX();		// Sphere coords
	float sy = this->getPos().getY();
	float sz = this->getPos().getZ();
	float r = this->getRadius();			// Radius

	float A = 1;
	float B = 2 * ((dx * ((double)rx - sx)) + (dy * ((double)ry - sy)) + (dz * ((double)rz - sz)));
	float C = pow((rx - sx), 2) + pow((ry - sy), 2) + pow((rz - sz), 2) - pow(r, 2);

	float discriminant = pow(B, 2) - (4.0 * A * C);
	float t, t1, t2;

	if (discriminant < 0) {
		// No real intersections
		return false;
	}
	else if (discriminant == 0) {
		// Tangential
		t = (-1.0 * B) / (2.0 * A);
	}
	else {
		// Two intersection points
		t1 = (static_cast<double>(-B) + sqrt(discriminant)) / (2.0 * A);
		t2 = (static_cast<double>(-B) - sqrt(discriminant)) / (2.0 * A);
		// Replace t with the closest parametric 
		if (t1 > 0 && t2 > 0) {
			t = std::min(t1, t2);
		} 
		else if (t1 < 0) {
			t = t2;
		}
		else {
			t = t1;
		}
	}

	// t could be negative
	if (t < 0) {
		return false;
	}

	// Set point and return true


	*intersection = Point3(ray.getPos() + (ray.getDirection() * t));
	
	return true;
}

Vector3 Sphere::getNormal(const Point3& point, const Scene& scene) const {
	return (point - pos);
}