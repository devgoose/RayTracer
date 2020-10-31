#pragma once
#include "SceneObject.h"

class Vector2;

#include <vector>

/**
 * Class for a triangle. 
 */
class Polygon :
  public SceneObject
{
private:
  std::vector<int> vertex_indices;
  std::vector<int> normal_indices;
  std::vector<int> tex_indices;
  bool isShaded;

  // Alpha/beta/gamma values of barycentric coordinates
  // These are set when an intersection is called

  // TODO: Refactor and put this in a Mesh class? 
  // Does each triangle need to hold extra memory this way

public:
  // Only vertex indices constructor
  Polygon(int v1, int v2, int v3, const int mtl_index_, const int tex_index_);
  // Vertex and texture indices
  Polygon(int v1, int v2, int v3, int t1, int t2, int t3, const int mtl_index_, const int tex_index_);
  /**
   * Complete constructor for a polygon.
   * 
   * @param v1 vertex index 
   * @param v2 vertex index
   * @param v3 vertex index
   * @param t1 texture index 
   * @param t2 texture index
   * @param t3 texture index
   * @param n1 normal index
   * @param n2 normal index
   * @param n3 normal index
   * @param mtl_index_ material index
   * @param tex_index_ texture index for the actual loaded texture image
   */
  Polygon(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, const int mtl_index_, const int tex_index_);
  
  void setTexIndices(int t1, int t2, int t3);
  void setNormalIndices(int n1, int n2, int n3);

  

  // bool isPointInPolygon(const Point3& intersection, float* alpha, float* beta, float* gamma, const Scene& scene);
  /**
   * Calcluates barycentric coordinates of a point and sets alpha, beta, and gamma values.
   * 
   * @param intersection Point from which to calculate coordinates
   * @param alpha barycentric coord to be set
   * @param beta barycentric coord to be set
   * @param gamma barycentric coord to be set
   * @param scene 
   */
  void getBarycentric(const Point3& intersection, float* alpha, float* beta, float* gamma, const Scene& scene) const;

  /**
   * Calculates diffuse at some point in the polygon
   * 
   * @param intersection point that is inside the polygon
   * @param scene
   * 
   * @return Color
   */
  Color getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const override;

  /**
   * Tests if a ray intersects the polygon, and sets the intersection point.
   * 
   * @param ray ray to test
   * @param intersection point that is set if intersection exists
   * @param scene
   * 
   * @return returns true if the ray intersects the polygon
   */
  bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const override;

  /**
   * Returns an interpolated normal from a point in the polygon.
   * 
   * @param point Point at which to calculate the normal
   * @param scene
   * 
   * @return new normal vector
   */
  Vector3 getNormal(const Point3& point, const Scene& scene) const override;
};

