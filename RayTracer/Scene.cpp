#include "Scene.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <regex>



using namespace std;

#include "Sphere.h"
#include "Polygon.h"
#include "PointLight.h"
#include "DirectionalLight.h"

Scene::~Scene() {
	deallocate_vector(objects);
	deallocate_vector(lights);
	deallocate_vector(vertexes);
	deallocate_vector(normals);
	deallocate_vector(texcoords);
	deallocate_vector(materials);
	deallocate_vector(textures);
}

template <class T>
void Scene::deallocate_vector(vector<T> vec) {
	for (unsigned int i = 0; i < vec.size(); i++) {
		delete vec[i];
	}
}

bool Scene::populate(string filename) {
	ifstream fin;
	fin.open(filename);
	if (!fin.is_open()) {
		cout << "Error opening file.";
		return false;
	}

	// Continue to read file

	string line;
	int line_number = 1;

	// .obj format has 1-indexed arrays, make a dummy at index 0
	vertexes.push_back(new Point3());
	normals.push_back(new Vector3());
	texcoords.push_back(new Vector2());

	// also push a material on for default if it is not provided
	materials.push_back(new Material());

	while (getline(fin, line)) {
		stringstream ss = stringstream(line);
		string keyword;

		ss >> keyword;

		cout << "Reading line: " << line_number << "   " << keyword << endl;

		if (keyword.compare("eye") == 0) {
			float x, y, z;
			ss >> x >> y >> z;
			if (!ss) {
				cout << "Failure reading input for eye on line: " << line_number << endl;
				return false;
			}
			eye = Point3(x, y, z);
		}
		else if (keyword.compare("updir") == 0) {
			float x, y, z;
			ss >> x >> y >> z;
			if (!ss) {
				cout << "Failure reading input for updir on line: " << line_number << endl;
				return false;
			}
			updir = Vector3(x, y, z).toUnit();
		}
		else if (keyword.compare("viewdir") == 0) {
			float x, y, z;
			ss >> x >> y >> z;
			if (!ss) {
				cout << "Failure reading input for viewdir on line: " << line_number << endl;
				return false;
			}
			viewdir = Vector3(x, y, z).toUnit();
		}
		else if (keyword.compare("hfov") == 0) {
			float hfov;
			ss >> hfov;
			if (!ss) {
				cout << "Failure reading input for hfov on line: " << line_number << endl;
				return false;
			}
			fov_h = hfov;
		}
		else if (keyword.compare("imsize") == 0) {
			float width, height;
			ss >> width >> height;
			if (!ss) {
				cout << "Failure reading input for imsize on line: " << line_number << endl;
				return false;
			}
			img_width = (int)width;
			img_height = (int)height;
		}
		else if (keyword.compare("bkgcolor") == 0) {
			float r, g, b;
			ss >> r >> g >> b;
			if (!ss) {
				cout << "Failure reading input for bkgcolor on line: " << line_number << endl;
				return false;
			}
			bkgcolor = Color(r, g, b);
		}
		else if (keyword.compare("mtlcolor") == 0) {
			float rd, gd, bd, rs, gs, bs, ka, kd, ks, n, alpha, eta;
			ss >> rd >> gd >> bd >> rs >> gs >> bs >> ka >> kd >> ks >> n >> alpha >> eta;
			if (!ss) {
				cout << "Failure reading input for mtlcolor on line: " << line_number << endl;
				return false;
			}
			if (alpha < 0 || alpha > 1 || eta < 1) {
				cout << "Failure reading input for mtlcolor on line: " << line_number << endl;
				cout << "Alpha or Eta values provided were invalid. \n";
				return false;
			}
			Color diff = Color(rd, gd, bd);
			Color spec = Color(rs, gs, bs);
			Material* mtlcolor = new Material(diff, spec, ka, kd, ks, n, alpha, eta);
			materials.push_back(mtlcolor);
		}
		else if (keyword.compare("texture") == 0) {
			string filename;
			ss >> filename;
			if (!ss) {
				cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
				return false;
			}
			Texture* t = new Texture();
			if (!t->load(filename)) {
				cout << "Error loading texture: " << filename << endl;
				return false;
			}
			textures.push_back(t);
		}
		else if (keyword.compare("light") == 0 || keyword.compare("attlight") == 0) {
			float x, y, z, r, g, b, c1, c2, c3;
			int w;
			if (keyword.compare("attlight") == 0) {
				ss >> x >> y >> z >> w >> r >> g >> b >> c1 >> c2 >> c3;
			}
			else {
				ss >> x >> y >> z >> w >> r >> g >> b;
			}
			if (!ss) {
				cout << "Failure reading input for color on line: " << line_number << endl;
				return false;
			}
			if (w != 0 && w != 1) {
				cout << "Failure reading input for color on line: " << line_number << endl;
				cout << "'w' value must be either 0 or 1" << endl;
				return false;
			}

			Color c = Color(r, g, b);
			Light* l;

			// Point Light
			if (w == 1) {
				l = new PointLight(c, Point3(x, y, z));
			}
			else {
				l = new DirectionalLight(c, Vector3(x, y, z));
			}

			// Attenuate light if applicable
			if (keyword.compare("attlight") == 0) {
				l->attenuate(c1, c2, c3);
			}
			lights.push_back(l);
		}
		else if (keyword.compare("depthcueing") == 0) {
			float r, g, b, amax, amin, distmax, distmin;
			ss >> r >> g >> b >> amax >> amin >> distmax >> distmin;
			if (!ss) {
				cout << "Failure reading input for depthcueing on line: " << line_number << endl;
				return false;
			}
			Color c = Color(r, g, b);
			depthCue = DepthCue(c, amax, amin, distmax, distmin);
			depthCued = true;
		}
		else if (keyword.compare("v") == 0) {
			float x, y, z;
			ss >> x >> y >> z;
			if (!ss) {
				cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
				return false;
			}
			Point3* p = new Point3(x, y, z);
			vertexes.push_back(p);
		} 
		else if (keyword.compare("vn") == 0) {
			float x, y, z;
			ss >> x >> y >> z;
			if (!ss) {
				cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
				return false;
			}
			Vector3* v = new Vector3(x, y, z);
			*v = v->toUnit();
			
			normals.push_back(v);
		}
		else if (keyword.compare("vt") == 0) {
			float x, y;
			ss >> x >> y;
			if (!ss) {
				cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
				return false;
			}
			Vector2* t = new Vector2(x, y);
			texcoords.push_back(t);
		}

		/* SCENEOBJECTS */

		else if (keyword.compare("sphere") == 0) {
			float x, y, z, r;
			ss >> x >> y >> z >> r;
			if (!ss) {
				cout << "Failure reading input for sphere on line: " << line_number << endl;
				return false;
			}
			Point3 p = Point3(x, y, z);
			SceneObject* so = new Sphere(p, r, materials.size() - 1, textures.size() - 1);
			objects.push_back(so);
		}
		else if (keyword.compare("f") == 0) {
			// Janky, but can determine what kind of face description it is by counting the number of slashes
			int slash_count = 0;
			for (int i = 0; i < line.length(); i++) {
				if (line[i] == '/') {
					slash_count++;
				}
			}

			int px, py, pz, nx, ny, nz, tx, ty, tz;

			// Basic face
			if (slash_count == 0) {
				ss >> px >> py >> pz;
				SceneObject* so = new Polygon(px, py, pz, materials.size() - 1, textures.size() - 1);
				objects.push_back(so);
			}
			// Just a texture
			else if (slash_count == 3) {
				string s = ss.str();
				replace(s.begin(), s.end(), '/', ' ');
				ss.str(s);

				ss >> s >> px >> tx >> py >> ty >> pz >> tz;
				SceneObject* so = new Polygon(px, py, pz, tx, ty, tz, materials.size() - 1, textures.size() - 1);
				objects.push_back(so);
			}
			// Point, texture, and normal,
			// Or Point and normal
			// Confusing syntax here
			else {
				// Copy the stream and test if it can handle  more than 6 floats
				// If not, then it should have 6 floats separated by two slashes each for normals and points
				// If more than that, it should have normals, texcoords, and points
				stringstream test = stringstream(ss.str());
				test >> px >> px >> px >> px >> px >> px >> px;
				
				// points and normals
				if (!test) {
					string s = ss.str();
					replace(s.begin(), s.end(), '/', ' ');
					ss.str(s);

					ss >> s >> px >> nx >> py >> ny >> pz >> nz;
					if (!ss) {
						cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
						return false;
					}
					Polygon* poly = new Polygon(px, py, pz, materials.size() - 1, textures.size() - 1);
					SceneObject* so = poly;
					poly->setNormalIndices(nx, ny, nz);
					objects.push_back(so);
				}
				// points textures and normals
				else {
					string s = ss.str();
					replace(s.begin(), s.end(), '/', ' ');
					ss.str(s);

					ss >> s >> px >> tx >> nx >> py >> ty >> ny >> pz >> tz >> nz;
					if (!ss) {
						cout << "Failure reading input for " + keyword + " on line: " << line_number << endl;
						return false;
					}
					SceneObject* so = new Polygon(px, py, pz, tx, ty, tz, nx, ny, nz, materials.size() - 1, textures.size() - 1);
					objects.push_back(so);
				}
			}
		}
		else {
			// Weird input, Continue
		}

		line_number++;
	}

	fin.close();

	return true;
}