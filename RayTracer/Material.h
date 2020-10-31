#pragma once
#include "Color.h"

/**
 * Extends from the Color class.
 * 
 * Holds more detailed information for basic material properties, such as 
 * diffuse, ambient, and specular values. 
 * 
 * Material properties are based on the Phong shading model. 
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

  float opacity;
  float refraction_index;

public:
  Material(const Color& diff, const Color& spec, float ka_, float kd_, float ks_, float exponent_, float opacity_, float refraction_index_);
  Material();
  Material(const Material& m);

  Color getDiffuse() const { return diffuse; }
  Color getSpecular() const { return specular; }
  float getAmbientCoef() const { return ka; }
  float getDiffuseCoef() const { return kd; }
  float getSpecularCoef() const { return ks; }
  float getExponent() const { return exponent; }
  float getOpacity() const { return opacity; }
  float getRefractionIndex() const { return refraction_index; }

};

