#pragma once
#include "SceneObject.h"

class TexCoord;

#include <vector>
class Polygon :
	public SceneObject
{
private:
	std::vector<int> vertex_indices;
	std::vector<int> normal_indices;
	std::vector<int> text_indices;
	bool isShaded;

public:
	// Only vertex indices constructor
	Polygon(int v1, int v2, int v3, const Material& color);
	// Vertex and texture indices
	Polygon(int v1, int v2, int v3, int t1, int t2, int t3, const Material& color);
	// Vertex, texture, and normal
	Polygon(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, const Material& color);
	
	// Set texture indices
	void setTextIndices(int t1, int t2, int t3);
	void setNormalIndices(int n1, int n2, int n3);

	Point3* getPoint(const Scene& scene, int index) const;

	Vector3* getNormal(const Scene& scene, int index) const;

	TexCoord* getTexcoord(const Scene& scene, int index) const;

	bool Intersect(const Ray& ray, Point3* intersection, const Scene& scene) override;

	Vector3 getNormal(const Point3& point, const Scene& scene) const override;
};

