#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <ctime>


#include "Ray.h"
#include "Scene.h"

int SAMPLE_RAYS = 1;
float JITTER_RADIUS = 0;


Ray Ray::jitter(float radius) {

	float rand_y = direction.getY() + ((float)(std::rand() / (float)RAND_MAX) * radius);
	float rand_z = direction.getZ() + ((float)(std::rand() / (float)RAND_MAX) * radius);
	float rand_x = direction.getX() + ((float)(std::rand() / (float)RAND_MAX) * radius);
	return Ray(pos, Vector3(rand_x, rand_y, rand_z).toUnit());
}

Color Ray::TraceRay(const Ray& ray, const Scene& scene) {
	Point3* closest_point = NULL;
	SceneObject* closest_object = NULL;

	float distance;
	for (int i = 0; i < scene.getNumObjects(); i++) {
		Point3 intersection;
		SceneObject* object;
		if (scene.getObject(i)->Intersect(ray, &intersection, scene)) {
			object = scene.getObject(i);
			if (closest_point == NULL) {
				closest_point = new Point3(intersection);
				closest_object = object;
			}
			else if (closest_point != NULL && 
				intersection.distance(ray.getPos()) < closest_point->distance(ray.getPos())) {
				closest_object = object;
				*closest_point = Point3(intersection);
			}
		}
	}

	// We now have closest_object and its intersection point, closest_point

	Color pixel_color;

	if (closest_point == NULL || closest_object == NULL) {
		// No intersections found! Return background color
		pixel_color = Color(scene.getBkgcolor());
	}
	else {
		pixel_color = Ray::ShadeRay(*closest_point, closest_object, scene);
	}

	return pixel_color;
}

Color Ray::ShadeRay(const Point3& point, const SceneObject* obj, const Scene& scene) {
	Color final_color;

	Material* material = scene.getMaterial(obj->getMaterialIndex());

	Color ambient_component = material->getDiffuse() * material->getAmbientCoef();

	// Loop through every light
	for (int i = 0; i < scene.getNumLights(); i++) {
		Light* light = scene.getLight(i);

		// Define vectors for Phong equation
		Vector3 light_dir;
		
		// Check all types of lights. Currently:
		// PointLight || DirectionalLight
		if (light->isPointLight()) {
			PointLight* l = (PointLight*)light;
			Point3 pos = l->getPos();
			light_dir = (pos - point).toUnit();
		}
		else {
			DirectionalLight* l = (DirectionalLight*)light;
			light_dir = (l->getDirection()).toUnit() * -1.0;
		}

		Vector3 normal = obj->getNormal(point, scene).toUnit();
		Vector3 view_dir = (scene.getEye() - point).toUnit();
		Vector3 halfway = (view_dir + light_dir).toUnit();

	
		// Loop through all objects, check for shadow
		Ray shadow_ray = Ray(point, light_dir);
		int obscured_rays = 0;
		int cast_rays = 0;
		Point3 intersection;
		for (int j = 0; j < SAMPLE_RAYS; j++) {

			// Sample a ray
			Ray sampled_shadow_ray = shadow_ray.jitter(JITTER_RADIUS);

			for (int i = 0; i < scene.getNumObjects(); i++) {
				SceneObject* cur_obj = scene.getObject(i);
				// Don't check for shadow of the object at the surface we are reflecting from
				if (cur_obj != obj) {
					if (cur_obj->Intersect(sampled_shadow_ray, &intersection, scene)) {
						// Intersection found between current ray and 
						// an object in the scene

						// For PointLights, must check if object intersection is in front of light
						if (light->isPointLight()) {
							PointLight* l = (PointLight*)light;
							float dist = point.distance(l->getPos());
							if (intersection.distance(point) < dist) {
								// Intersection is in front of the light, increment obscured rays
								obscured_rays++;
							}
							else {
								continue;
							}
						}
						// Or, automatically increment if directional light
						else {
							obscured_rays++;
						}
						// Because an intersection found, calculate 
						// next ray
						break;
					}
				}
			}

			cast_rays++;
		}

		float shadow_flag = (float)(cast_rays - obscured_rays) / (float)cast_rays;
		// DEBUG
		if (shadow_flag != 0) {
			int a = 1;
		}
		float attenuation_flag = 1;
		if (light->isAttenuated()) {
			PointLight* pl = (PointLight*)light;
			attenuation_flag = light->getAttenuationVal(point.distance(pl->getPos()));
		}
		// float shadow_flag = 1;
		Color diffuse_component = 
			material->getDiffuse() *
			material->getDiffuseCoef() * 
			std::max((float)0.0, normal.dot(light_dir));

		Color specular_component =
			material->getSpecular() *
			material->getSpecularCoef() *
			pow(std::max((float)0, normal.dot(halfway)), material->getExponent());


		final_color = final_color + (light->getColor() * (diffuse_component + specular_component) * shadow_flag * attenuation_flag);
	}

	// Finally, add the ambient component

	final_color = final_color + ambient_component;
	
	if (scene.isDepthCued()) {
		float dist = scene.getEye().distance(point);
		float depthCueFactor = scene.getDepthCue().getDepthCueFactor(dist);
		final_color = (final_color * depthCueFactor)
			+ (scene.getDepthCue().getColor() * (1.0 - depthCueFactor));
	}

	return final_color;
}


