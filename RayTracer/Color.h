#pragma once

#include <iostream>
#include <fstream>


class Color
{
private:
	float r;
	float g;
	float b;

public:
	Color() {
		r = 0;
		g = 0;
		b = 0;
	}

	Color(float r_, float g_, float b_) {
		r = r_;
		g = g_;
		b = b_;
	}

	Color(const Color& c);

	float getR() const { return r; }
	float getG() const { return g; }

	Color operator+(const Color& c);
	Color operator*(const float c);
	Color operator*(const Color& c);

	/*
	Clamps all color values to 1 if they are greater than one.
	*/
	void clamp();

	/*
	Prints out the color so that it is formated for PPM.
	*/
	friend std::ostream& operator<<(std::ostream& out, const Color& color);

};







