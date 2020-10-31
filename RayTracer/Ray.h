#pragma once

#include "Point3.h"
#include "Color.h"

/** 
 * Class that holds a ray in 3d space. 
 */
class Scene;
class SceneObject;

class Ray
{
private:
  Point3 pos;
  Vector3 direction;

public:
  /**
   * Constructor takes a position and direction.
   */
  Ray(const Point3 &pos_, const Vector3 &direction_) {
    pos = pos_;
    direction = direction_.toUnit();
  }

  Point3 getPos() const { return pos; }
  Vector3 getDirection() const { return direction; }

  /**
   * Enum for identifying what kind of ray this ray is.
   * 
   * INSIDE
   */
  enum RayType {
    INSIDE, 	/**Ray is inside of a material (was refracted) */
    OUTSIDE		/**Ray is outside of a material (was reflected) */
    };

  /**
   * Adds a small bit of variation to the direction of the array. Does not
   * modify this ray.
   * 
   * @param radius radius of the orthogonal circle at the end of the ray
   * 
   * @return new Ray that is slightly offset
   */
  Ray jitter(float radius);

  /**
   * Main ray tracing function. Given a ray that is shot from an eye, reflects the ray throughout the scene
   * and returns a color to be eventually drawn/recorded. 
   * 
   * If an object is intersected by this ray, calls ShadeRay().
   * 
   * @param ray Ray to trace
   * @param scene
   * @param depth Recursive depth counter
   * @param fresnel_prodcut Accumulator for transparency
   * @param origin_obj points to the object that the ray originates from. If comes from the eye, this is null
   */
  static Color TraceRay(const Ray& ray, const Scene& scene, int depth, float fresnel_product, const SceneObject* origin_obj);

  /** 
  * Main function for tracing rays at intersection points and for determining the color
  * of a pixel if it is not to be the scene's background color. 
  * 
  * Loops through every light in the scene and draws a ray from the intersection point provided
  * to those lights to determine if it intersects any object or not. For soft shadows, ray.cpp
  * provides some globals that can be toggled when building though this should probably be 
  * modified in the future so it can be taken in as an input file.
  * 
  * For soft shadows, jitter() is called many times and a sample of rays is shot from each intersection point
  * so that an object in shadow can have a blend between its ambient color and its lit up color.
  * 
  * A fourth argument depth should start at 0 with the initial call.
  * @param incident_ray ray that reflects off obj towards something else
  * @param point point from which the ray comes from
  * @param obj obj that the ray comes from
  * @param scene
  * @param depth recursive depth counter
  * @param fresnel_product recursive transparency accumulator
  *
  * @return Color to be shaded from this ray 
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
  /**
   * Helper functino for ray tracing. Tests if the ray intersects anything in the scene other than itself,
   * and if so, it sets closest_obj to point to the closest object.
   * 
   * @param ray ray to test for intersections 
   * @param origin_obj object the array comes from, this cannot be intersected
   * @param closest_obj should be a NULL pointer, is pointed to closest object if intersections exist
   * @param closest_point should be NULL, is set to the intersection point on the closest object
   * @param scene
   * 
   * @return true if closest_obj and closest_point were set
   */
  static bool IntersectAllObjects(const Ray& ray, const RayType ray_type, const SceneObject* origin_obj, SceneObject** closest_obj, Point3* closest_point, const Scene& scene);

  /*
  Begin ray tracing given a ray, assumed to be coming from air and with an origin
  at the scene's eye.

  incident_ray:	INCOMING ray, facing the direction of the trace
  scene:			current scene
  */
  /**
   * Begin ray tracing given a ray, assumed to be originating in air, and with
   * an origin at the scene's eye. 
   * 
   * @param incident_ray incoming ray, facing the direction of the trace
   * @param scene
   * 
   * @return Color as a result of the trace
   */
  static Color TraceRayFromOrigin(const Ray& incident_ray, const Scene& scene);

  /**
   * Recursive functino that can be used for tracing all reflected and refracted rays. Calculates
   * the color from local illumination, and is then recursively called on each other ray in the Phong
   * model.
   * 
   * @param incident_ray incoming ray, facing direction of the trace
   * @param origin_obj object that the ray comes from/is travelling through
   * @param medium used to calculate index of refraction, this is always air unless a refracted
   * ray enters an object or exits an object
   * @param scene
   * @param depth recursive depth counter
   * @param multiplier cumulative multiplier applied to each color that is recursively traced by a ray, and
   * will return black if this multiplier is below MINIMUM_COLOR_ATTRIBUTION
   * @param ray_type type of the incident ray
   * 
   * @return Color traced 
   */
  static Color RecursiveTraceRay(const Ray& incident_ray, const SceneObject* origin_obj, const SceneObject* medium, const Scene& scene, int depth, float multiplier, RayType ray_type);
 
  /**
   * Calculates local illumination from a point, incident ray, and an object's material properties, using the lights in the scene
   * and the Phong illumination model.
   * 
   * @param incident_dir direction of an incident ray
   * @param ray_type 
   * @param point point of collision with the object
   * @param local_obj object on which illumination is calculated
   * @param scene
   * 
   * @return Color representing local illumination
   */
  static Color GetLocalIllumination(const Vector3& incident_dir, const RayType ray_type, const Point3 point, const SceneObject* local_obj, const Scene& scene);
};



