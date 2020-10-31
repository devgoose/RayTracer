#pragma once
#include <iostream>
#include <string>
#include <vector>

/**
 * Class that holds an image texture.
 */
class Color;

class Texture
{
private:
  /**
   * 2D array of colors. 
   */
  std::vector< std::vector<Color*> > texture;
  int height, width;
public:
  Texture();
  /**
   * Constructor that initializes the texture with the file. 
   * 
   * @param filename filepath for a .ppm texture file
   */
  Texture(std::string filename);
  ~Texture();

  int getHeight() const { return height; }
  int getWidth() const { return width; }

  /**
   * Retrieves a color at the texture coordinates.
   * 
   * @param i
   * @param j
   * 
   * @return color
   */
  Color* getColorAtCoord(int i, int j) const;

  /**
   * Loads the texture from a file
   * 
   * @param filename file path for a .ppm texture file
   * 
   * @return true if successfully loaded
   */
  bool load(std::string filename);
};

