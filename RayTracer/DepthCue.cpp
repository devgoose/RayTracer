#include "DepthCue.h"

DepthCue::DepthCue() {
  depthColor = Color();
  a_max = 1;
  a_min = 0;
  dist_max = 1000000;
  dist_min = 1000000;
}

DepthCue::DepthCue(const Color& depthColor_, float amax, float amin, 
                  float distmax, float distmin) {
  depthColor = depthColor_;
  a_max = amax;
  a_min = amin;
  dist_max = distmax;
  dist_min = distmin;
}

float DepthCue::getDepthCueFactor(float view_dist) const {
  if (view_dist <= dist_min) {
    return a_max;
  } else if (view_dist >= dist_max) {
    return a_min;
  } else {
    return a_min + ((a_max - a_min) * 
          ((dist_max - view_dist) / (dist_max - dist_min)));
  }
}