#include "Texture.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>


#include "Color.h"

Texture::Texture() {
  height = 0;
  width = 0;
}

Texture::Texture(std::string filename) {
  load(filename);
}

Texture::~Texture() {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      delete texture[i][j];
    }
  }
}

Color* Texture::getColorAtCoord(int i, int j) const {
  return texture[i][j];
}

bool Texture::load(std::string filename) {
  std::ifstream fin;
  fin.open(filename);

  if (!fin.is_open()) {
    std::cout << "Error opening " << filename << " to load texture.\n";
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
    return false;
  }

  // Header successfully read. Now read colors
  height = h;
  width = w;

  std::string line;
  int line_number = 0;

  // Get file size for progress bar
  struct stat stat_buf;
  stat(filename.c_str(), &stat_buf);
  long size = stat_buf.st_size;
  

  for (int i = 0; i < height; i++) {
    std::vector<Color*> row;
    for (int j = 0; j < width; j++) {
      float r, g, b;
      Color* c;

      fin >> r >> g >> b;

      if (fin) {
        c = new Color(r / max_value, g / max_value, b / max_value);
      }
      else {
        // std::cout << "Error parsing " << filename << " on line " << line_number << std::endl;
        // return false;
        c = new Color();

      }
      row.push_back(c);
      line_number++;

    }
    texture.push_back(row);

    ///////////////
    // PROGRESS BAR
    // TODO: PUT IN UTIL
    ////////////////
    int bar_size = 25;
    float percentage = (float)i / (float)height;
    printf("\rProgress reading %s: ", filename.c_str());
    for (int k = 0; k < bar_size; k++) {
      if (k <= percentage * bar_size) {
        // output a block character
        printf("%c", 219);
      }
      else {
        printf("-");
      }
    }
    fflush(stdout);
  }
  std::cout << std::endl;
  
  fin.close();

  return true;
}