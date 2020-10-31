#pragma once
#include "Color.h"

/**
 * Holds information for a scene's depthcueing settings, if enabled. 
 */
class DepthCue
{
private:
/**
 * Color to interpolate far away objects to. 
 */
  Color depthColor;
  /**
   * Maxmimum amount of interpolation. 
   */
  float a_max;
  /**
   * Minimum amount of interpolation. 
   */
  float a_min;
  /**
   * Maximum distance that interpolation occurs. 
   */
  float dist_max;
  /**
   * Minimum distance that interpolation occurs. 
   */
  float dist_min;

public:
  DepthCue();
  DepthCue(const Color& depthColor_, float amax, float amin, float distmax, float distmin);

  Color getColor() const { return depthColor; }

  /**
   * Calculates the depth cue factor to be applied to some object at 
   * some distance away.
   * 
   * @param view_dist distance away to be calculated
   * 
   * @return depth cue factor
   */
  float getDepthCueFactor(float view_dist) const;
};

