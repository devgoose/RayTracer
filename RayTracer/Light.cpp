#include "Light.h"

Light::Light() {
	color = Color();
	attenuated = false;
	c1 = 0;
	c2 = 0;
	c3 = 0;

}

Light::Light(const Color& color_) {
	color = Color(color_);
	attenuated = false;
	c1 = 0;
	c2 = 0;
	c3 = 0;
}

void Light::attenuate(float c1_, float c2_, float c3_) {
	attenuated = true;
	c1 = c1_;
	c2 = c2_;
	c3 = c3_;
}

float Light::getAttenuationVal(float distance) {
	if (this->isPointLight()) {
		return 1.0 / (c1 + (distance * c2) + (pow(distance, 2) * c3));
	}
	else {
		return 1;
	}
}

