// Copyright 2020 
// Author: Jake Walters

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>

#include "Scene.h"
#include "Ray.h"
#include "ViewWindow.h"

using namespace std;

string output_filename = "image.ppm";

int main(int argc, char* argv[]) {
  if (argc < 2) {
    cout << "Usage: ./render <filename>" << endl;
    return 0;
  }

  Scene scene;
  if (!scene.populate(argv[1])) {
    cout << "Error parsing file. Closing" << endl;
    return 0;
  }
  else {
    cout << "Succesfully scanned input file\n";
  }

  srand(time(0));

  ViewWindow view_window(scene);

  ofstream fout;
  fout.open(output_filename);

  if (!fout.is_open()) {
    cout << "Error opening output file. Closing." << endl;
    return 0;
  }

  fout << "P3\n" << scene.getWidth() << " " << scene.getHeight() << endl << "255" << endl;

  printf("\n\n\n---------------\nRay Tracing...\n---------------\n\n");
  int j = 0;
  for (int i = 0; i < scene.getHeight(); i++) {
    for (j = 0; j < scene.getWidth(); j++) {
      // DEBUGGING
      int cur_pix = i * scene.getWidth() + j;
      int middle = (scene.getWidth() * scene.getHeight() / 2) + (scene.getWidth() / 2);
      if (i == 264 && j == 144) {
        int break_here = 1;
      }

      Vector3 direction = Vector3(view_window.getPoint(i, j) - scene.getEye());
      Ray ray = Ray(scene.getEye(), direction);

      // Begin ray tracing at 0 depth
      // Reflection = 1, assuming camera is in air
      // Pass in NULL as current object because the first ray originates from eye
      // Color pixel_color = Ray::TraceRay(ray, scene, 0, 1.0, NULL);
      Color pixel_color = Ray::TraceRayFromOrigin(ray, scene);
      pixel_color.clamp();
      fout << pixel_color << endl;
      if (i  == 60 && j == 238) {
        int a = 0;
      }

      

    }
    ///////////////
    // PROGRESS BAR
    // TODO: PUT IN UTIL
    ////////////////
    int bar_size = 25;
    float percentage = (float)i / (float)scene.getHeight();
    printf("\rProgress: ");
    for (int k = 0; k < bar_size; k++) {
      if (k <= percentage * bar_size) {
        // output a block character
        printf("%c", 219);
      }
      else {
        printf("-");
      }
    }
    // DEBUG
    printf("i: %d", i);
    fflush(stdout);
    
  }

  fout.close();
  
  cout << "\n\nFinished!\n";

  return 0;
}