#pragma once

#include <iostream>
#include <fstream>
#include <vector>

#include "SceneObject.h"
#include "Color.h"
#include "Light.h"
#include "PointLight.h"
#include "DirectionalLight.h"
#include "DepthCue.h"
#include "TexCoord.h"

class SceneObject;

/*
Class that holds all information in a scene.
Includes references to the camera and output image settings,
global depthcueing settings, and all objects and lights
located in the scene. 
*/
class Scene
{
private:
	Point3 eye;
	Vector3 viewdir;
	Vector3 updir;

	int img_width = 0;
	int img_height = 0;
	float fov_h = 0;
	
	Color bkgcolor;
	std::vector<SceneObject*> objects;
	std::vector<Light*> lights;

	// Meshes, defined as just one mesh for now

	std::vector<Point3*> vertexes;
	std::vector<Vector3*> normals;
	std::vector<TexCoord*> texcoords;


	DepthCue depthCue;
	bool depthCued = false;

public:
	Scene() { }
	~Scene();

	Point3 getEye() const { return eye; }
	Vector3 getViewdir() const { return viewdir; }
	Vector3 getUpdir() const { return updir; }
	int getWidth() const { return img_width; }
	int getHeight() const { return img_height; }
	float getHfov() const { return fov_h; }
	Color getBkgcolor() const { return bkgcolor; }
	Light* getLight(int i) const { return lights[i]; }
	SceneObject* getObject(int i) const { return objects[i]; }
	int getNumObjects() const { return objects.size(); }
	int getNumLights() const { return lights.size(); }
	DepthCue getDepthCue() const { return depthCue; }
	bool isDepthCued() const { return depthCued; }
	Point3* getVertex(int i) const { return vertexes[i]; }
	Vector3* getNormal(int i) const { return normals[i]; }
	TexCoord* getTexcoord(int i) const { return texcoords[i]; }


	
	/*
	Fills the scene with from the input file provided.
	*/
	bool populate(std::string filename);
};
