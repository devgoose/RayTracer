#pragma once
#include "Light.h"
#include "Vector3.h"

/**
 * Directional lights are lights with a direction. Duh
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

  /**
   * Easy way to tell what type a light is. 
   */
  bool isPointLight() { return false; }
};

