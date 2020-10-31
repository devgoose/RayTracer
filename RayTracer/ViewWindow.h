#pragma once

#include <vector>

#include "Point3.h"
class Scene;

/**
 * Class for the projection plane from which rays are shot through. 
 */
class ViewWindow
{
private:
  Point3 ul;
  Point3 ur;
  Point3 ll;
  Point3 lr;
  std::vector< std::vector <Point3> > view_lattice;

public:
  ViewWindow(const Scene& scene);

  /**
   * Retrieves a point in the view lattice.
   * 
   * @param x
   * @param y
   * 
   * @return point
   */
  Point3 getPoint(int x, int y);

  /**
   * Fills in the four corners of the windows from the settings 
   * in the scene.
   * 
   * @param scene
   */
  void fillCorners(const Scene& scene);

  /**
   * Fills in the lattice of points given the four corners
   * and the scene. fillCorners() should be called before this.
   * 
   * @param scene
   */
  void fillLattice(const Scene& scene);

};

