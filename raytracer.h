#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <cmath>

#include "three_d_vector.h"
#include "ray.h"
#include "surface.h"
#include "light.h"

using namespace std;

class RayTracer{
	public:
		vector<Surface*> surfaces;
		vector<Light*> lights;
		ThreeDVector* trace(Ray* ray);
		bool hits_surface(Ray* ray);
		RayTracer(vector<Surface*> surfaces, vector<Light*> lights);
		~RayTracer();
	private:
		ThreeDVector* calculate_diffuse(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit);
		ThreeDVector* calculate_diffuse_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* diffuse, ThreeDVector* normal);
		ThreeDVector* calculate_specular(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit, ThreeDVector* view_direction);
		ThreeDVector* calculate_specular_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* specular, ThreeDVector* normal, ThreeDVector* view_direction, float power_coefficient);
};

#endif