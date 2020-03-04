#include "PointLight.h"

PointLight::PointLight() {
	pos = Point3();
}

PointLight::PointLight(const Color& color, const Point3& pos_)
	: Light(color) {
	pos = pos_;
}