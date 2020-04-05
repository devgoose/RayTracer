#pragma once

#include "Ray.h"
#include "Material.h"
#include "Scene.h"

const float MIN_INTERSECT_DIST = 0.01;

/*
Base Object for holding any object in the scene.
Good for creating basic geometry like spheres, but can also be applied
to something like a cylinder, cube, etc. 
*/

class SceneObject
{
protected:
	int mtl_index;
	int tex_index;

public:
	SceneObject();

	SceneObject(const SceneObject& obj);

	SceneObject(const int m) { mtl_index = m; tex_index = -1; }

	SceneObject(const int m, const int t) { mtl_index = m; tex_index = t; }

	int getTextureIndex() const { return tex_index; }
	int getMaterialIndex() const { return mtl_index; }

	void setTextureIndex(const int tex_index_);
	void setMaterialIndex(const int mtl_index_);

	bool isTextured() const;

	virtual Color getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const = 0;
	virtual bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const = 0;

	/*
	Abstract function that returns the normal of an object at a given point,
	if necessary. 

	Returns a UNIT vector
	*/
	virtual Vector3 getNormal(const Point3& point, const Scene& scene) const = 0;
};