#pragma once

#include "Ray.h"
#include "Material.h"
#include "Scene.h"

/*
Base Object for holding any object in the scene.
Good for creating basic geometry like spheres, but can also be applied
to something like a cylinder, cube, etc. 
*/

class SceneObject
{
protected:
	Point3 pos;
	Material mtlcolor;
public:
	SceneObject() { pos = Point3(); mtlcolor = Material(); }

	SceneObject(const SceneObject& obj);

	Point3 getPos() const { return pos; }
	Material getMaterial() const { return mtlcolor; }

	virtual bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) = 0;
	virtual Vector3 getNormal(const Point3& point, const Scene& scene) const = 0;
};

