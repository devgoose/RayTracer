#pragma once

#include "SceneObject.h"


class Sphere : public SceneObject
{
private:
	Point3 pos;
	float radius;

public:
	Sphere();

	Sphere(const Sphere& sphere);

	Sphere(const Point3& pos_, float radius_, const int mtl_index_);

	Sphere(const Point3& pos_, float radius_, const int mtl_index_, const int tex_index_);

	Point3 getPos() const { return pos; }

	float getRadius() const{ return radius; }

	// Virtual override functions
	bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const override;
	Vector3 getNormal(const Point3& point, const Scene& scene) const override;
	Color getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const override;
	
};

