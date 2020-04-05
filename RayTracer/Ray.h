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
	Enum for identifying how a ray originated. 
	Not part of a ray object, just passed through functions.

	INITIAL:	originated from the scene's eye
	REFLECTED:	ray has been reflected off a surface
	REFRACTED:	ray has been refracted off/through a surface
				can be exiting or entering
				on each trace on a refracted ray, this is toggled between
				OUTSIDE and INSIDE
	*/
	enum RayType {INSIDE, OUTSIDE};

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
	static Color TraceRay(const Ray& ray, const Scene& scene, int depth, float fresnel_product, const SceneObject* origin_obj);

	/*
	Main function for tracing rays at intersection points and for determining the color
	of a pixel if it is not to be the scene's background color. 
	
	Loops through every light in the scene and draws a ray from the intersection point provided
	to those lights to determine if it intersects any object or not. For soft shadows, ray.cpp
	provides some globals that can be toggled when building though this should probably be 
	modified in the future so it can be taken in as an input file.
	
	For soft shadows, jitter() is called many times and a sample of rays is shot from each intersection point
	so that an object in shadow can have a blend between its ambient color and its lit up color.

	A fourth argument depth should start at 0 with the initial call.
	*/
	static Color ShadeRay(const Ray& incident_ray, const Point3& point, const SceneObject* obj, const Scene& scene, int depth, float fresnel_product);

	/*
	Helper function for ray tracing. Tests if the ray intersects anything in the scene other than itself,
	and if so, it sets closets_obj to the closest object. 

	ray:			ray which is tested for intersections
					this should not be an incident ray
	origin_obj:		obj the ray starts from--can't be intersected with
					if NULL, assumed to be from the eye
	closest_obj:	should be NULL, is pointed to the closest object intersected
	closest_point:	should be NULL, same thing
	scene:			current scene
	*/
	static bool IntersectAllObjects(const Ray& ray, const RayType ray_type, const SceneObject* origin_obj, SceneObject** closest_obj, Point3* closest_point, const Scene& scene);

	/*
	Begin ray tracing given a ray, assumed to be coming from air and with an origin
	at the scene's eye.

	incident_ray:	INCOMING ray, facing the direction of the trace
	scene:			current scene
	*/
	static Color TraceRayFromOrigin(const Ray& incident_ray, const Scene& scene);

	/*
	Recursive function that can be used for tracing all reflected and refracted rays.
	Calculates the color of the local illumination, and is then recursively called
	on each other calculated ray in the Phong model.

	incident_ray:	INCOMING ray, facing the direction of the trace
	origin_obj:		object that the ray comes from/is traveling through
	medium:			used to calculate the index of refraction, 
					this is always air until a refracted ray enters 
					an object or exits an object.

					Could keep a stack of mediums, but for now it is just 
					toggled to and from air.

	scene:			current scene
	depth:			current recursive depth. each call this increases by 1
	multiplier:		a cumulative multipler. During each trace of a reflected or refracted
					ray, there is a multiplier applied to the color returned. This multiplier
					is calculated differently for refraction and reflection, and is accumlated
					each recursive step. Alongside depth, this is an additional recursion termination
					factor, and the trace will terminate and return a black color when this multiplier
					is below a certain threshold in MIN_COLOR_ATTRIBUTION.
	*/
	static Color RecursiveTraceRay(const Ray& incident_ray, const SceneObject* origin_obj, const SceneObject* medium, const Scene& scene, int depth, float multiplier, RayType ray_type);

	static Color GetLocalIllumination(const Vector3& incident_dir, const RayType ray_type, const Point3 point, const SceneObject* local_obj, const Scene& scene);
};



