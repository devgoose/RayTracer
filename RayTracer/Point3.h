#pragma once

#include "Vector3.h"

/*
Class for holding a point in 3d space.
*/
class Point3
{
private:
	float x;
	float y;
	float z;

public:
	Point3() {
		x = 0;
		y = 0;
		z = 0;
	}

	Point3(float x_, float y_, float z_) {
		x = x_;
		y = y_;
		z = z_;
	}

	Point3(const Point3& p2) {
		x = p2.x;
		y = p2.y;
		z = p2.z;
	}

	float getX() { return x; }
	float getY() { return y; }
	float getZ() { return z; }

	void setX(float x_) { x = x_; }
	void setY(float y_) { y = y_; }
	void setZ(float z_) { z = z_; }

	Point3 operator+(const Vector3& v);
	Point3 operator-(const Vector3& v);

	Vector3 operator-(const Point3& p) const;

	float distance(const Point3& p) const;
};
