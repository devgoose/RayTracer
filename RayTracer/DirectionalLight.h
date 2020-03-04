#pragma once
#include "Light.h"
#include "Vector3.h"

/*
Inherits from the Light class.

Directional Lights have a single direction.
*/
class DirectionalLight :
	public Light
{
private:
	Vector3 direction;

public:
	DirectionalLight();

	DirectionalLight(const Color& color, const Vector3& direction_);

	Vector3 getDirection() const{  return direction; }
	bool isPointLight() { return false; }
};

