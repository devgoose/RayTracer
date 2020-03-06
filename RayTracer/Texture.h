#pragma once
#include <iostream>
#include <string>
#include <vector>

class Color;

class Texture
{
private:
	// Vector of colors, each corresponding to a pixel defined a ppm image
	std::vector< std::vector<Color*> > texture;
	int height, width;
public:
	Texture();
	Texture(std::string filename);
	~Texture();

	int getHeight() const { return height; }
	int getWidth() const { return width; }

	Color* getColorAtCoord(int i, int j) const;

	bool load(std::string filename);
};

