#include <math.h>
#include <cstdlib>
#include <algorithm>
#include <ctime>


#include "Ray.h"
#include "Scene.h"

int SHADOW_SAMPLE_RAYS = 1;
float SHADOW_JITTER_RADIUS = 0;

int REFLECTION_SAMPLE_RAYS = 1;
float REFLECTION_JITTER_RADIUS = 0;

int MAX_RECURSIVE_DEPTH = 5;
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
		if (normal.dot(light_dir) < 0) {
			// normal = normal.reverse();
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
				if (cur_obj != obj) {
					if (cur_obj->Intersect(sampled_shadow_ray, &intersection, scene)) {
						if (light->isPointLight()) {
							PointLight* l = (PointLight*)light;
							float dist = point.distance(l->getPos());
							if (intersection.distance(point) < dist) {
								obscured_rays++;
							}
							else {
								continue;
							}
						}
						else {
							obscured_rays++;
						}
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
			std::max((float)0.0, (normal.dot(light_dir)));

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
			distributed_reflected_color = distributed_reflected_color + (TraceRay(jittered_ray, scene, depth + 1, fresnel_product * fresnel, obj) * fresnel);
		}
		distributed_reflected_color = distributed_reflected_color / REFLECTION_SAMPLE_RAYS;
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



// Initial call to trace ray from the viewpoint
// Origin object is null, which the function handles as having air-like refractiveness 
Color Ray::TraceRayFromOrigin(const Ray& incident_ray, const Scene& scene) {
	return RecursiveTraceRay(incident_ray, NULL, NULL, scene, 0, 1, Ray::OUTSIDE);
}

Color Ray::RecursiveTraceRay(const Ray& incident_ray, const SceneObject* origin_obj, const SceneObject* medium, const Scene& scene, int depth, float multiplier, Ray::RayType ray_type) {
	// Recursion end condition, don't return anything that can contribute to the final color
	if (depth >= MAX_RECURSIVE_DEPTH ||
		multiplier < MIN_COLOR_ATTRIBUTION) {
		return Color(0, 0, 0);
	}

	SceneObject* intersection_object = NULL;
	Point3 intersection_point;

	// Find the closest object and intersection point that the ray hits, or if it hits the background, 
	if (!Ray::IntersectAllObjects(incident_ray, ray_type, origin_obj, &intersection_object, &intersection_point, scene) || intersection_object == NULL) {
		return scene.getBkgcolor();
	}

	Material* intersection_material = scene.getMaterial(intersection_object->getMaterialIndex());
	Vector3 normal = ray_type == INSIDE ? intersection_object->getNormal(intersection_point, scene).reverse()
		: intersection_object->getNormal(intersection_point, scene);
	Vector3 incident_dir = incident_ray.getDirection().reverse().toUnit();
	Color ambient;
	Color local;
	Color reflected_ray_color;
	Color refracted_ray_color;

	// Extract indices of refraction, but origin_obj can be null
	float eta_i = 1; // default air
	float eta_t = intersection_material->getRefractionIndex();
	if (medium != NULL) {
		eta_i = scene.getMaterial(medium->getMaterialIndex())->getRefractionIndex();
	}

	float opacity = intersection_material->getOpacity();
	float ndoti = normal.dot(incident_dir); // save this, its used a lot
	float iof = eta_i / eta_t; // ratio of indices of refraction
	float reflectance = pow((eta_t - 1) / (eta_t + 1), 2); // Fo in the equation
	float fresnel = reflectance + ((1.0 - reflectance) * pow(1.0 - ndoti, 5));
	float refracted_coef = (1.0 - fresnel) * (1.0 - opacity);

	Ray reflected_ray = Ray(intersection_point, (normal * (normal.dot(incident_dir) * 2.0) - incident_dir).toUnit());

	float under_sqrt = 1.0 - (pow(iof, 2) * (1.0 - pow(ndoti, 2)));
	Ray refracted_ray = Ray(Point3(), Vector3());

	// For a refracted ray, toggle its next medium.
	// If the medium is currently inside of an object, the next medium it should exit
	// the object and will be air. If the medium is currently air, then the intersected
	// object is the new medium.
	SceneObject* refracted_medium = medium == NULL ? intersection_object : NULL;

	if (under_sqrt > 0) {
		refracted_ray = Ray(intersection_point,
			((normal.reverse() * sqrt(under_sqrt)) +
			(((normal * ndoti) - incident_dir) * iof)).toUnit());
	}

	// Now we have all the puzzle pieces -- just calculate all the colors
	// In the case of total internal reflection, ignore the refracted color
	ambient = intersection_object->getDiffuseColorAtPoint(intersection_point, scene) * intersection_material->getAmbientCoef();
	local = Ray::GetLocalIllumination(incident_dir, ray_type, intersection_point, intersection_object, scene);
	reflected_ray_color = RecursiveTraceRay(reflected_ray, intersection_object, medium, scene, depth + 1, multiplier * fresnel, ray_type) * fresnel;
	refracted_ray_color = under_sqrt > 0 ?
		RecursiveTraceRay(refracted_ray, intersection_object, refracted_medium, scene, depth + 1, multiplier * refracted_coef,
			ray_type == INSIDE ? OUTSIDE : INSIDE) * refracted_coef :
		Color();

	return ambient + local + reflected_ray_color + refracted_ray_color;
}


bool Ray::IntersectAllObjects(const Ray& ray, const RayType ray_type, const SceneObject* origin_obj, SceneObject** closest_obj, Point3* closest_point, const Scene& scene)  {
	// Temporary objects to hold results of the intersection
	Point3 intersection;
	SceneObject* cur_obj = NULL;
	bool intersects = false;

	// Define multi-use vars here for speed
	int num_objects = scene.getNumObjects();
	Point3 origin = ray.getPos();

	// Loop through every object
	for (int i = 0; i < num_objects; i++) {
		cur_obj = scene.getObject(i);
		// Test for an intersections OTHER than origin_obj
		// Only update closest point/object when this is the first intersection,
		// or the current intersection is closer than the previous
		//
		// Depending on ray type, this ray could be currently inside a sphere and 
		// in that case, it is allowed to intersect itself at a decent distance. 
		if (cur_obj->Intersect(ray, &intersection, scene) && 
			(cur_obj != origin_obj || 
			(ray_type == INSIDE && intersection.distance(origin) > MIN_INTERSECT_DIST))
			) {
			intersects = true;
			if (*closest_obj == NULL || intersection.distance(origin) < closest_point->distance(origin)) {
				*closest_obj = cur_obj;
				*closest_point = intersection;
			}
		}
	}
	return intersects;
}

Color Ray::GetLocalIllumination(const Vector3& incident_dir, const RayType ray_type, const Point3 point, const SceneObject* local_obj, const Scene& scene) {
	Color local_color = Color();

	Material* material = scene.getMaterial(local_obj->getMaterialIndex());
	int num_lights = scene.getNumLights();
	int num_objects = scene.getNumObjects();
	Vector3 view_dir = (scene.getEye() - point).toUnit();

	// Get N vector
	// Flip it around if it's facing the wrong direction
	Vector3 normal = local_obj->getNormal(point, scene);
	if (incident_dir.dot(normal) < 0) {
		// normal = normal.reverse();
	}

	for (int i = 0; i < num_lights; i++) {
		Vector3 light_dir;
		Vector3 halfway;
		// Get L vector
		Light* light = scene.getLight(i);
		if (light->isPointLight()) {
			PointLight* l = (PointLight*)light;
			light_dir = (l->getPos() - point).toUnit();
		}
		else {
			DirectionalLight* l = (DirectionalLight*)light;
			light_dir = (l->getDirection()).toUnit().reverse();
		}

		// Get H vector (halfway)
		halfway = (light_dir + view_dir).toUnit();

		// Calculate shadow flag
		// For each jittered shadow ray, need to check for intersection with every object
		// On an intersection found, its alpha opacity value is compounded (for a maximum of 1)
		float shadow_flag = SHADOW_SAMPLE_RAYS;
		for (int j = 0; j < SHADOW_SAMPLE_RAYS; j++) {
			float cumulative_alpha = 0;
			Ray shadow_ray = Ray(point, light_dir).jitter(SHADOW_JITTER_RADIUS);

			for (int k = 0; k < num_objects; k++) {
				SceneObject* cur_obj = scene.getObject(k);
				Point3 intersection;
				if (local_obj != cur_obj && 
					cur_obj->Intersect(shadow_ray, &intersection, scene)) {
					cumulative_alpha += scene.getMaterial(cur_obj->getMaterialIndex())->getOpacity();
					// If this is above 1, the ray is fully in shadow
					if (cumulative_alpha >= 1) {
						cumulative_alpha = 1;
						break;
					}
				}
			}

			shadow_flag -= cumulative_alpha;
		}

		shadow_flag = shadow_flag / (float)SHADOW_SAMPLE_RAYS;
		// Account for attenuation
		float attenuation_flag = 1;
		if (light->isAttenuated()) {
			PointLight* pl = (PointLight*)light;
			attenuation_flag = light->getAttenuationVal(point.distance(pl->getPos()));
		}

		Color diffuse_component =
			local_obj->getDiffuseColorAtPoint(point, scene) *
			material->getDiffuseCoef() *
			std::max((float)0.0, normal.dot(light_dir));

		Color specular_component =
			material->getSpecular() *
			material->getSpecularCoef() *
			pow(std::max((float)0, normal.dot(halfway)), material->getExponent());


		local_color = local_color + (light->getColor() * (diffuse_component + specular_component) * shadow_flag * attenuation_flag);
	}
	return local_color;
}

