#include "Polygon.h"

#include <cmath>

Polygon::Polygon(int v1, int v2, int v3, const int mtl_index_, const int tex_index_) {
  vertex_indices.push_back(v1);
  vertex_indices.push_back(v2);
  vertex_indices.push_back(v3);
  mtl_index = mtl_index_;
  tex_index = tex_index_;
  isShaded = false;
}

Polygon::Polygon(int v1, int v2, int v3, int t1, int t2, int t3, const int mtl_index_, const int tex_index_) {
  vertex_indices.push_back(v1);
  vertex_indices.push_back(v2);
  vertex_indices.push_back(v3);
  tex_indices.push_back(t1);
  tex_indices.push_back(t2);
  tex_indices.push_back(t3);
  mtl_index = mtl_index_;
  tex_index = tex_index_;
  isShaded = false;
}

Polygon::Polygon(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, const int mtl_index_, const int tex_index_) {
  vertex_indices.push_back(v1);
  vertex_indices.push_back(v2);
  vertex_indices.push_back(v3);
  tex_indices.push_back(t1);
  tex_indices.push_back(t2);
  tex_indices.push_back(t3);
  normal_indices.push_back(n1);
  normal_indices.push_back(n2);
  normal_indices.push_back(n3);
  mtl_index = mtl_index_;
  tex_index = tex_index_;
  isShaded = true;
}

void Polygon::setTexIndices(int t1, int t2, int t3) {
  tex_indices.push_back(t1);
  tex_indices.push_back(t2);
  tex_indices.push_back(t3);
}

void Polygon::setNormalIndices(int n1, int n2, int n3) {
  normal_indices.push_back(n1);
  normal_indices.push_back(n2);
  normal_indices.push_back(n3);
  isShaded = true;
}


bool Polygon::Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const{
  // Determine if ray intersects plane at all
  // Plane equation Ax + By + Cz + D = 0
  // A = Nx, B = Ny, C = Nz
  Point3 p0 = *(scene.getVertex(vertex_indices[0]));
  Point3 p1 = *(scene.getVertex(vertex_indices[1]));
  Point3 p2 = *(scene.getVertex(vertex_indices[2]));
  Vector3 e1 = p1 - p0;
  Vector3 e2 = p2 - p0;

  Vector3 normal = e1.cross(e2).toUnit();

  // Detect intersections if the triangle is facing the other way? 
  

  float A = normal.getX();
  float B = normal.getY();
  float C = normal.getZ();
  float D = (float)-1.0 * ((A * p0.getX()) + (B * p0.getY()) + (C * p0.getZ()));

  float denominator = (A * ray.getDirection().getX()) +
    (B * ray.getDirection().getY()) +
    (C * ray.getDirection().getZ());

  if (denominator == 0) {
    return false;
  }

  float t = (float)-1.0 *	
    (	(A * ray.getPos().getX())
    + (B * ray.getPos().getY())
    + (C * ray.getPos().getZ()) 
    + D	)
    / denominator;

  if (t < 0) {
    return false;
  }

  // Plane is in front of ray, get intersection point
  Point3 plane_intersection = ray.getPos() + (ray.getDirection() * t);

  // Calculate if plane_intersection is inside of the polygon
  // Get barycentric coordinates
  float alpha = 0;
  float beta = 0;
  float gamma = 0;
  getBarycentric(plane_intersection, &alpha, &beta, &gamma, scene);

  // Check if alpha, beta and gamma are within range
  if (alpha <= 0 || alpha >= 1) {
    return false;
  }
  if (beta <= 0 || beta >= 1) {
    return false;
  }
  if (gamma <= 0 || gamma >= 1) {
    return false;
  }

  // Made it here, so plane_intersection is a good point
  *intersection = Point3(plane_intersection);
  return true;
}

void Polygon::getBarycentric(const Point3& intersection, float* alpha, float* beta, float* gamma, const Scene& scene) const {
  Point3 p0 = *(scene.getVertex(vertex_indices[0]));
  Point3 p1 = *(scene.getVertex(vertex_indices[1]));
  Point3 p2 = *(scene.getVertex(vertex_indices[2]));
  Vector3 e1 = p1 - p0;
  Vector3 e2 = p2 - p0;

  Vector3 ep = intersection - p0;
  float d11 = e1.dot(e1);
  float d12 = e1.dot(e2);
  float d22 = e2.dot(e2);
  float dp1 = ep.dot(e1);
  float dp2 = ep.dot(e2);

  float E = (d11 * d22) - (d12 * d12);
  float Eb = (d22 * dp1) - (d12 * dp2);
  float Ey = (d11 * dp2) - (d12 * dp1);

  *beta = Eb / E;
  *gamma = Ey / E;
  *alpha = (float)1.0 - (*beta + *gamma);
}

Vector3 Polygon::getNormal(const Point3& point, const Scene& scene) const {
  Vector3 normal;
  if (isShaded) {
    // normal = (alpha*n0 + beta*n1 + gamma*n2).toUnit()
    // assume that Intersection has already been found, and 
    // therefore alpha/beta/gamma set
    float alpha = 0;
    float beta = 0;
    float gamma = 0;
    getBarycentric(point, &alpha, &beta, &gamma, scene);

    Vector3* n0 = scene.getNormal(normal_indices[0]);
    Vector3* n1 = scene.getNormal(normal_indices[1]);
    Vector3* n2 = scene.getNormal(normal_indices[2]);

    normal = (*n0 * alpha) + (*n1 * beta) + (*n2 * gamma);
    normal = normal.toUnit();
  }
  else {
    Point3 p0 = *(scene.getVertex(vertex_indices[0]));
    Point3 p1 = *(scene.getVertex(vertex_indices[1]));
    Point3 p2 = *(scene.getVertex(vertex_indices[2]));
    Vector3 e1 = p1 - p0;
    Vector3 e2 = p2 - p0;

    normal = e1.cross(e2).toUnit();
  }
  return normal;
}

Color Polygon::getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const {
  // If object is not textured, just return its material diffuse color
  if (!isTextured()) {
    return scene.getMaterial(mtl_index)->getDiffuse();
  }

  // Otherwise, compute the texture coordinate u, v and find the nearest
  // color in the texture
  float alpha, beta, gamma;
  getBarycentric(intersection, &alpha, &beta, &gamma, scene);

  Vector2* p0 = scene.getTexcoord(tex_indices[0]);
  Vector2* p1 = scene.getTexcoord(tex_indices[1]);
  Vector2* p2 = scene.getTexcoord(tex_indices[2]);

  float u = (float)(
    (alpha * (float)p0->getX()) +
    (beta * (float)p1->getX()) +
    (gamma * (float)p2->getX())
    );

  float v = (float)(
    (alpha * (float)p0->getY()) +
    (beta * (float)p1->getY()) +
    (gamma * (float)p2->getY())
    );

  Texture* object_texture = scene.getTexture(tex_index);

  int texture_height = object_texture->getHeight();
  int texture_width = object_texture->getWidth();

  int i = round(u * (texture_width - 1));
  int j = round(v * (texture_height - 1));

  Color* c = object_texture->getColorAtCoord(j, i);
  return Color(*c);
}