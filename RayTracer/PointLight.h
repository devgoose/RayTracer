#pragma once
#include "Light.h"
#include "Point3.h"

/*
Inherits from Light class.

PointLights have a position in 3d space.
*/
class PointLight :
	public Light
{
private:
	Point3 pos;

public:
	PointLight();

	PointLight(const Color& color, const Point3& pos_);

	Point3 getPos() const { return pos; }
	bool isPointLight() { return true; }
};

