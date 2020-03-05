
#include "Polygon.h"

Polygon::Polygon(int v1, int v2, int v3, const Material& color) {
	vertex_indices.push_back(v1);
	vertex_indices.push_back(v2);
	vertex_indices.push_back(v3);
	mtlcolor = color;
	isShaded = false;
}

Polygon::Polygon(int v1, int v2, int v3, int t1, int t2, int t3, const Material& color) {
	vertex_indices.push_back(v1);
	vertex_indices.push_back(v2);
	vertex_indices.push_back(v3);
	text_indices.push_back(t1);
	text_indices.push_back(t2);
	text_indices.push_back(t3);
	mtlcolor = color;
	isShaded = true;
}

Polygon::Polygon(int v1, int v2, int v3, int t1, int t2, int t3, int n1, int n2, int n3, const Material& color) {
	vertex_indices.push_back(v1);
	vertex_indices.push_back(v2);
	vertex_indices.push_back(v3);
	text_indices.push_back(t1);
	text_indices.push_back(t2);
	text_indices.push_back(t3);
	normal_indices.push_back(n1);
	normal_indices.push_back(n2);
	normal_indices.push_back(n3);
	mtlcolor = color;
	isShaded = true;
}

void Polygon::setTextIndices(int t1, int t2, int t3) {
	text_indices.push_back(t1);
	text_indices.push_back(t2);
	text_indices.push_back(t3);
}

void Polygon::setNormalIndices(int n1, int n2, int n3) {
	normal_indices.push_back(n1);
	normal_indices.push_back(n2);
	normal_indices.push_back(n3);
	isShaded = true;
}


bool Polygon::Intersect(const Ray& ray, Point3* intersection, const Scene& scene) {
	// Determine if ray intersects plane at all
	// Plane equation Ax + By + Cz + D = 0
	// A = Nx, B = Ny, C = Nz
	Point3 p0 = *(scene.getVertex(vertex_indices[0]));
	Point3 p1 = *(scene.getVertex(vertex_indices[1]));
	Point3 p2 = *(scene.getVertex(vertex_indices[2]));
	Vector3 e1 = p1 - p0;
	Vector3 e2 = p2 - p0;

	Vector3 normal = e1.cross(e2).toUnit();

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
	*alpha = 1.0 - ((double)*beta + (double)*gamma);
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