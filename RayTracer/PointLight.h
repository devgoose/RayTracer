#pragma once
#include "Light.h"
#include "Point3.h"

/**
 * Inherits from light class. Acts as a point in space. 
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

