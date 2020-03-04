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

	srand(time(0));

	ViewWindow view_window(scene);

	ofstream fout;
	fout.open(output_filename);

	if (!fout.is_open()) {
		cout << "Error opening output file. Closing." << endl;
		return 0;
	}

	fout << "P3\n" << scene.getWidth() << " " << scene.getHeight() << endl << "255" << endl;

	for (int i = 0; i < scene.getHeight(); i++) {
		for (int j = 0; j < scene.getWidth(); j++) {
			Vector3 direction = Vector3(view_window.getPoint(i, j) - scene.getEye());
			Ray ray = Ray(scene.getEye(), direction);

			Color pixel_color = Ray::TraceRay(ray, scene);
			pixel_color.clamp();
			fout << pixel_color << endl;
		}
	}

	fout.close();
	
	cout << "Finished\n";

	return 0;
}