#pragma once

#include "Point3.h"
#include "Color.h"

/*
Holds a Ray in 3d space, with a point and a vector.
*/
class Scene;
class SceneObject;

class Ray
{
private:
	Point3 pos;
	Vector3 direction;

public:
	Ray(const Point3 &pos_, const Vector3 &direction_) {
		pos = pos_;
		direction = direction_.toUnit();
	}

	Point3 getPos() const { return pos; }
	Vector3 getDirection() const { return direction; }

	/*
	Takes the "radius" given to it and applies that random value to the ray 
	so that it varies by a small amount.

	This returns a new ray.
	*/
	Ray jitter(float radius);


	/*
	Main ray tracing function. Takes a ray that is shot from the eye to a point
	in the view window, and the scene so that the scene's objects and lights can be grabbed.
	
	Uses this ray and the objects in the scene to determine if it collides with any objects, and
	if it does, calls ShadeRay.
	*/
	static Color TraceRay(const Ray& ray, const Scene& scene);

	/*
	Main function for tracing rays at intersection points and for determining the color
	of a pixel if it is not to be the scene's background color. 
	
	Loops through every light in the scene and draws a ray from the intersection point provided
	to those lights to determine if it intersects any object or not. For soft shadows, ray.cpp
	provides some globals that can be toggled when building though this should probably be 
	modified in the future so it can be taken in as an input file.
	
	For soft shadows, jitter() is called many times and a sample of rays is shot from each intersection point
	so that an object in shadow can have a blend between its ambient color and its lit up color.
	*/
	static Color ShadeRay(const Point3& point, const SceneObject* obj, const Scene& scene);

	
};



