#pragma once

#include <vector>

#include "Point3.h"
class Scene;

/*
Class that holds the information for the ViewWindows from
which to shoot rays through. 
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

	Point3 getPoint(int x, int y);

	/*
	Fills the four corners with the points at each corner 
	of the window.
	*/
	void fillCorners(const Scene& scene);

	/*
	Loops through every point in the point lattice and fills it in.
	*/
	void fillLattice(const Scene& scene);

};

