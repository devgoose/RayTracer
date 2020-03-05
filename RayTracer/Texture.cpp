#include "Texture.h"

#include <iostream>
#include <fstream>

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


}