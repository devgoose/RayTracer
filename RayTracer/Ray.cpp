#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <ctime>


#include "Ray.h"
#include "Scene.h"

int SHADOW_SAMPLE_RAYS = 1;
float SHADOW_JITTER_RADIUS = .1;

int REFLECTION_SAMPLE_RAYS = 10;
float REFLECTION_JITTER_RADIUS = .1;

int MAX_RECURSIVE_DEPTH = 10;
float MIN_COLOR_ATTRIBUTION = 0.001;


Ray Ray::jitter(float radius) {

	float rand_y = direction.getY() + (((float)(std::rand() / (float)RAND_MAX) - 0.5) * (radius * 2.0));
	float rand_z = direction.getZ() + (((float)(std::rand() / (float)RAND_MAX) - 0.5) * (radius * 2.0));
	float rand_x = direction.getX() + (((float)(std::rand() / (float)RAND_MAX) - 0.5) * (radius * 2.0));
	return Ray(pos, Vector3(rand_x, rand_y, rand_z).toUnit());
}

Color Ray::TraceRay(const Ray& ray, const Scene& scene, int depth, float fresnel_product, const SceneObject* origin_obj) {
	Point3* closest_point = NULL;
	SceneObject* closest_object = NULL;

	float distance;
	for (int i = 0; i < scene.getNumObjects(); i++) {
		Point3 intersection;
		SceneObject* object;

		// Don't consider the object the ray is originating from
		if (scene.getObject(i) == origin_obj) {
			continue;
		}

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
		pixel_color = Ray::ShadeRay(ray, *closest_point, closest_object, scene, depth, fresnel_product);
	}

	return pixel_color;
}

Color Ray::ShadeRay(const Ray& incident_ray, const Point3& point, const SceneObject* obj, const Scene& scene, int depth, float fresnel_product) {
	Color final_color;
	Color local_color;

	Material* material = scene.getMaterial(obj->getMaterialIndex());
	Color ambient_component = obj->getDiffuseColorAtPoint(point, scene) * material->getAmbientCoef();

	Vector3 normal = obj->getNormal(point, scene).toUnit();
	Vector3 incident_dir = (incident_ray.getDirection() * -1.0).toUnit();

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
		
		// Get halfway vector
		Vector3 halfway = (incident_dir + light_dir).toUnit();

		// Loop through all objects, check for shadow
		Ray shadow_ray = Ray(point, light_dir);
		int obscured_rays = 0;
		int cast_rays = 0;
		Point3 intersection;
		for (int j = 0; j < SHADOW_SAMPLE_RAYS; j++) {

			// Sample a ray
			Ray sampled_shadow_ray = shadow_ray.jitter(SHADOW_JITTER_RADIUS);

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
			Color ambient_component = material->getDiffuse() * material->getAmbientCoef();
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


		Color diffuse_component = 
			obj->getDiffuseColorAtPoint(point, scene) *
			material->getDiffuseCoef() * 
			std::max((float)0.0, normal.dot(light_dir));

		Color specular_component =
			material->getSpecular() *
			material->getSpecularCoef() *
			pow(std::max((float)0, normal.dot(halfway)), material->getExponent());


		local_color = local_color + (light->getColor() * (diffuse_component + specular_component) * shadow_flag * attenuation_flag);
	}

	// Add the ambient component
	local_color = local_color + ambient_component;

	// Recursively ray trace, only if we have not yet reached depth,
	// or the Fr ^ depth * color is less than the minimum color value

	// Test if reflected color is above minimum color value
	float eta = material->getRefractionIndex();
	float fo = pow((eta - 1.0) / (eta + 1.0), 2);

	float fresnel = fo + ((1.0 - fo) * pow((1.0 - normal.dot(incident_dir)), 5));
	Color reflective_color = local_color * fresnel_product;

	if (depth < MAX_RECURSIVE_DEPTH &&
		(reflective_color.getR() > MIN_COLOR_ATTRIBUTION ||
			reflective_color.getG() > MIN_COLOR_ATTRIBUTION ||
			reflective_color.getB() > MIN_COLOR_ATTRIBUTION)
		) {

		// Reflection ray = 2(N dot I)N - I, starting from intersection 
		Ray reflection_ray = Ray(point, (normal * (normal.dot(incident_dir) * 2.0) - incident_dir).toUnit());

		// TEST
		// Distributed reflections
		Color distributed_reflected_color = Color();
		for (int j = 0; j < REFLECTION_SAMPLE_RAYS; j++) {
			Ray jittered_ray = reflection_ray.jitter(REFLECTION_JITTER_RADIUS);
			distributed_reflected_color = distributed_reflected_color + (TraceRay(reflection_ray, scene, depth + 1, fresnel_product * fresnel, obj) * fresnel);
		}
		distributed_reflected_color = distributed_reflected_color / REFLECTION_JITTER_RADIUS;
		final_color = local_color + distributed_reflected_color;

		
		// final_color = local_color + (TraceRay(reflection_ray, scene, depth + 1, fresnel_product * fresnel, obj) * fresnel);
	}
	else {
		final_color = local_color;
	}


	
	if (scene.isDepthCued()) {
		float dist = scene.getEye().distance(point);
		float depthCueFactor = scene.getDepthCue().getDepthCueFactor(dist);
		final_color = (final_color * depthCueFactor)
			+ (scene.getDepthCue().getColor() * (1.0 - depthCueFactor));
	}

	return final_color;
}


