#include "Texture.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Color.h"


Texture::Texture(std::string filename) {
	load(filename);
}

Texture::~Texture() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; i < width; i++) {
			delete texture[i][j];
		}
	}
}

bool Texture::load(std::string filename) {
	std::ifstream fin;
	fin.open(filename);

	if (!fin.is_open()) {
		std::cout << "Error opening file to load texture.\n";
		return false;
	}

	std::string header;
	int w, h;
	float max_value;
	
	fin >> header;

	if (!fin || header.compare("P3")) {
		std::cout << "Error on " + filename + " header.\n";
		return false;
	}

	fin >> w >> h >> max_value;
	if (!fin || w < 0 || h < 0 || max_value < 0) {
		std::cout << "Error with height/width/pixel maximum value in " + filename << std::endl;
	}

	// Header successfully read. Now read colors
	
	std::string line;
	int line_number = 0;

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			float r, g, b;
			Color* c;

			if (getline(fin, line)) {
				std::stringstream ss = std::stringstream(line);

				ss >> r >> g >> b;

				if (!ss) {
					std::cout << "Error parsing " << filename << " on line " << line_number << std::endl;
					return false;
				}
				 c = new Color(r / max_value, g / max_value, b / max_value);
			}
			else {
				c = new Color();
			}
			texture[i].push_back(c);
		}
	}
	
	fin.close();
	return true;
}