#include <math.h>
#include <algorithm>

#include "Sphere.h"

const float M_PI = (float)3.141592653;

Sphere::Sphere() 
	: SceneObject() {
	radius = 0;
	pos = Point3();
}

Sphere::Sphere(const Sphere& sphere) {
	radius = sphere.radius;
	mtl_index = sphere.mtl_index;
	tex_index = sphere.tex_index;
	pos = sphere.pos;
}

Sphere::Sphere(const Point3& pos_, float radius_, const int mtl_index_) {
	pos = pos_;
	radius = radius_;
	mtl_index = mtl_index_;
	tex_index = -1;
}

Sphere::Sphere(const Point3& pos_, float radius_, const int mtl_index_, const int tex_index_) {
	pos = pos_;
	radius = radius_;
	mtl_index = mtl_index_;
	tex_index = tex_index_;
}

bool Sphere::Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const {
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
	float B = (float)2.0 * ((dx * (rx - sx)) + (dy * (ry - sy)) + (dz * (rz - sz)));
	float C = pow((rx - sx), 2) + pow((ry - sy), 2) + pow((rz - sz), 2) - pow(r, 2);

	float discriminant = pow(B, 2) - ((float)4.0 * A * C);
	float t, t1, t2;

	if (discriminant < 0) {
		// No real intersections
		return false;
	}
	else if (discriminant == 0) {
		// Tangential
		t = ((float)-1.0 * B) / ((float)2.0 * A);
	}
	else {
		// Two intersection points
		t1 = (float)((float)(-B) + (float)sqrt(discriminant)) / (float)((float)2.0 * A);
		t2 = (float)((float)(-B) - (float)sqrt(discriminant)) / (float)((float)2.0 * A);
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
	return (point - pos).toUnit();
}

Color Sphere::getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const {
	// If object is not textured, just return its material diffuse color
	if (!isTextured()) {
		return scene.getMaterial(mtl_index)->getDiffuse();
	}

	// Otherwise, calculate texture coordinate according to such:
	// phi(v) = acos(N_z)
	// theta(u) = atan(N_y / N_x)
	// normalize phi and theta to (0,1)
	// 
	// Many ways to map this
	//
	// v = phi / pi
	// u = balanced_theta / 2pi
	//		balance theta by adding 2pi when it is negative
	//
	// where N is the UNIT vector from center to intersection

	Vector3 N = (intersection - pos).toUnit();
	float phi = (float)std::acos(N.getY());
	float theta = (float)std::atan2(N.getZ(), -N.getX());
	if (theta < 0) {
		theta += ((float)2.0 * M_PI);
	}

	float v = phi / M_PI;


	float u = (theta) / ((float)2.0 * M_PI);

	Texture* object_texture = scene.getTexture(tex_index);

	int texture_height = object_texture->getHeight();
	int texture_width = object_texture->getWidth();

	int j = round(u * (texture_width - 1));
	int i = round(v * (texture_height - 1));

	Color* c = object_texture->getColorAtCoord(i, j);
	return Color(*c);
}