#pragma once
#include "Color.h"

/*
Holds information for a scene's depthcueing if it is enabled.
*/
class DepthCue
{
private:
	Color depthColor;
	float a_max;
	float a_min;
	float dist_max;
	float dist_min;

public:
	DepthCue();
	DepthCue(const Color& depthColor_, float amax, float amin, float distmax, float distmin);

	Color getColor() const { return depthColor; }

	/*
	Calculates the depth cue factor to be multiplied to a color 
	based on the distance from the eye provided.
	*/
	float getDepthCueFactor(float view_dist) const;
};

