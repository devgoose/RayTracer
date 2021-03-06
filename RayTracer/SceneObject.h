#pragma once

#include "Ray.h"
#include "Material.h"
#include "Scene.h"

const float MIN_INTERSECT_DIST = 0.01;

/**
 * Base class for holding any object in the scene. Good for creating basic geometry like spheres, but 
 * can also be applied to something like a cylinder, cube, etc. 
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

  /**
   * Full constructor that initializes the object with a texture and
   * material.
   * 
   * @param m material index
   * @param t texture index
   */
  SceneObject(const int m, const int t) { mtl_index = m; tex_index = t; }

  int getTextureIndex() const { return tex_index; }
  int getMaterialIndex() const { return mtl_index; }

  void setTextureIndex(const int tex_index_);
  void setMaterialIndex(const int mtl_index_);

  /**
   * @return true if the object has a texture
   */
  bool isTextured() const;

  /**
   * Calculates the diffuse color at a point on the object.
   * 
   * @param intersection point at which to calculate color
   * @param scene
   * 
   * @return diffuse color
   */
  virtual Color getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const = 0;

  /**
   * Tests if a ray intersects this object, and if so, sets the intersection point.
   * 
   * @param ray ray to test
   * @param intersection point to set if intersection is found
   * @param scene
   * 
   * @return true if intersection is found and the point is set
   */
  virtual bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const = 0;

  /**
   * Gets the normal vector at a certain point.
   * 
   * @param point point at which to get the normal vector
   * @param scene
   * 
   * @return unit normal vector at point
   */
  virtual Vector3 getNormal(const Point3& point, const Scene& scene) const = 0;
};