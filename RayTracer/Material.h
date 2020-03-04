#pragma once
#include "Color.h"

/*
Extends from Color class.

Holds some more detailed information for a sceneobject's material properties.

This allows an object to be fully customizable for ambient, diffuse, and specular lighting.
*/
class Material :
	public Color
{
private:
	Color diffuse;
	Color specular;

	float ka;
	float kd;
	float ks;
	float exponent;

public:
	Material(const Color& diff, const Color& spec, float ka_, float kd_, float ks_, float exponent_);
	Material();
	Material(const Material& m);

	Color getDiffuse() const { return diffuse; }
	Color getSpecular() const { return specular; }
	float getAmbientCoef() const { return ka; }
	float getDiffuseCoef() const { return kd; }
	float getSpecularCoef() const { return ks; }
	float getExponent() const { return exponent; }

};

