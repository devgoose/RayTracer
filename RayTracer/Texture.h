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
	Texture(std::string filename);
	~Texture();

	bool load(std::string filename);


};

