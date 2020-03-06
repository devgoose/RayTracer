#pragma once
#include "SceneObject.h"

class Vector2;

#include <vector>
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
	// Vertex, texture, and normal
	Polygon(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, const int mtl_index_, const int tex_index_);
	
	// Set texture indices
	void setTexIndices(int t1, int t2, int t3);
	void setNormalIndices(int n1, int n2, int n3);

	

	// bool isPointInPolygon(const Point3& intersection, float* alpha, float* beta, float* gamma, const Scene& scene);
	void getBarycentric(const Point3& intersection, float* alpha, float* beta, float* gamma, const Scene& scene) const;

	// Virtual override functions
	Color getDiffuseColorAtPoint(const Point3& intersection, const Scene& scene) const override;
	bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) const override;
	Vector3 getNormal(const Point3& point, const Scene& scene) const override;
};

