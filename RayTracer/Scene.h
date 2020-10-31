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
#include "Vector2.h"
#include "Texture.h"

class SceneObject;

/**
 * Class that contains all information for a 3D scene. Includes references
 * to the camera, image settings, depthcueing settings, and all objects and lights
 * located in the scene. 
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

  /** 
   * Array of all vertexes in the scene. Currently there is no "mesh" implementation. 
   */
  std::vector<Point3*> vertexes;
  /** 
   * Array of all normal vectors in the scene. 
   */
  std::vector<Vector3*> normals;
  /**
   * Array of all texture coordinates in the scene. 
   */
  std::vector<Vector2*> texcoords;
  
  /**
   * Array of all materials in the scene. 
   */
  std::vector<Material*> materials;
  /**
   * Array of all textures in the scene. 
   */
  std::vector<Texture*> textures;

  /**
   * DepthCue object for depthcueing settings, if enabled. 
   */
  DepthCue depthCue;
  bool depthCued = false;

  /**
   * Helper method for deleting vectors of objects. 
   */
  template <class T>
  void deallocate_vector(std::vector<T> vec);

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
  Point3* getVertex(int i) const { return vertexes[i]; }
  Vector3* getNormal(int i) const { return normals[i]; }
  Vector2* getTexcoord(int i) const { return texcoords[i]; }
  Material* getMaterial(int i) const { return materials[i]; }
  Texture* getTexture(int i) const { return textures[i]; }

  int getNumObjects() const { return (int)objects.size(); }
  int getNumLights() const { return (int)lights.size(); }
  DepthCue getDepthCue() const { return depthCue; }
  bool isDepthCued() const { return depthCued; }
  
  /**
   * Initializes, or "populates" the scene with data included in the file.
   * 
   * @param filename name of the input config file
   * 
   * @return true if successful reading of the file
   */
  bool populate(std::string filename);
};

