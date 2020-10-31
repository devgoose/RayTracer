
#pragma once

/**
 * Skeleton of a 2D vector library. 
 */
class Vector2
{
private:
  float x;
  float y;

public:
  Vector2() { x = 0; y = 0; }
  Vector2(float x_, float y_) {
    x = x_;
    y = y_;
  }

  float getX() const { return x; }
  float getY() const { return y; }
};

