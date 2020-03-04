#pragma once

#include "Color.h"

/*
Parent class for a light.
*/
class Light
{
private:
	Color color;
	bool attenuated;
	float c1;
	float c2;
	float c3;

public:

	Light();

	Light(const Color& color_);

	// Light(const Color& color_, bool attenuated_, float c1_, float c2_, float c3_);

	/*
	Can flip a light's attenuation on if it should be attenuated. 
	Otherwise, the attenuation values aren't used. 
	*/
	void attenuate(float c1_, float c2_, float c3_);

	Color getColor() const { return color; }

	/*
	If the light is attenuated, this calculates the attenuation value. 
	Otherwise, it returns 1.
	*/
	float getAttenuationVal(float distance);

	bool isAttenuated() const { return attenuated; }

	virtual bool isPointLight() = 0;
};

