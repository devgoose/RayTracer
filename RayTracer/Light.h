#pragma once

#include "Color.h"

/**
 * A light. 
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

  /**
   * Enables attenuation on this light. 
   * 
   * @param c1_ constant attenuation
   * @param c2_ linear attenuation
   * @param c3_ quadratic attenuation
   */
  void attenuate(float c1_, float c2_, float c3_);

  Color getColor() const { return color; }

  /**
   * A light's return value is multiplied by some color to be drawn.
   * 
   * @code atten = 1.0 / (c1 + (d * c2) + (d * d * c3))
   * 
   * @return if light is attenuated, returns the formula above. Else, returns 1. 
   */
  float getAttenuationVal(float distance);

  bool isAttenuated() const { return attenuated; }

  virtual bool isPointLight() = 0;
};

