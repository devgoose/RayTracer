#pragma once

#include <iostream>
#include <fstream>

/**
 * A color with RGB values expressed in ranges from 0 to 1. 
 */
class Color
{
 private:
  float r;
  float g;
  float b;

 public:
  /** 
   * Default constructorm makes black. 
   */
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

  /**
   * Copy constructor. 
   */
  Color(const Color& c);

  float getR() const { return r; }
  float getG() const { return g; }
  float getB() const { return b; }

  Color operator+(const Color& c);
  Color operator*(const float c);
  Color operator*(const Color& c);
  Color operator/(const float c);
  Color operator/(const Color& c);

  void green() 	{ r = 0.0; g = 1.0; b = 0.0; }
  void red() 		{ r = 1.0; g = 0.0; b = 0.0; }
  void blue() 	{ r = 0.0; g = 0.0; b = 1.0; }
  void white()	{ r = 1.0; g = 1.0; b = 1.0; }
  void black()	{ r = 0.0; g = 0.0; b = 0.0; }
  void orange() { r = 1.0; g = 0.5; b = 0.0; }
  void yellow() { r = 1.0; g = 1.0; b = 0.0; }
  void cyan() 	{ r = 0.0; g = 1.0; b = 1.0; }
  void purple() { r = 0.5; g = 0.0; b = 1.0; }
  void pink() 	{ r = 1.0; g = 0.0; b = 1.0; }

  /**
   * Clamps all color values to 1.
   */
  void clamp();

  /**
   * Output stream overload so colors can be easily printed out. 
   * 
   * @code cout << color
   * 
   * @param out an output stream
   * @param color 
   */
  friend std::ostream& operator<<(std::ostream& out, const Color& color);

};







