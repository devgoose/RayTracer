#pragma once

#include "SceneObject.h"


class Sphere : public SceneObject
{
private:
	float radius;

public:
	Sphere() {
		radius = 0;
	}

	Sphere(const Sphere& sphere);

	Sphere(const Point3& pos_, float radius_, const Material& mtlcolor_) {
		pos = pos_;
		radius = radius_;
		mtlcolor = mtlcolor_;
	}

	float getRadius() const{ return radius; }

	bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) override;
	Vector3 getNormal(const Point3& point, const Scene& scene) const override;
	
};

