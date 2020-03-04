#include "Point3.h"
#include <math.h>

Point3 Point3::operator+(const Vector3& v) {
	return Point3(x + v.getX(), y + v.getY(), z + v.getZ());
}

Point3 Point3::operator-(const Vector3& v) {
	return Point3(x - v.getX(), y - v.getY(), z - v.getZ());
}

Vector3 Point3::operator-(const Point3& p) const {
	return Vector3(x - p.x, y - p.y, z - p.z);
}

float Point3::distance(const Point3& p) const {
	return (float)fabs(sqrt(pow((double)((double)p.x - x), 2) +
		pow((double)((double)p.y - y), 2) +
		pow((double)((double)p.z - z), 2)));
}