#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>
#include <cmath>

#include "three_d_vector.h"
#include "ray.h"
#include "surface.h"
#include "light.h"
#include "AABBNode.h"

using namespace std;

class RayTracer{
	public:
		vector<Surface*> surfaces;
		vector<Light*> lights;
		bool soft_shadow;
		AABBNode* acceleration_node;
		ThreeDVector* trace(Ray* ray, int recursive_depth, Surface* except_surface);
		bool hits_surface(Ray* ray, Surface* except_surface);
		int num_hits_light(vector<Ray*> rays, Surface* surface);
		RayTracer(vector<Surface*> surfaces, vector<Light*> lights, bool soft_shadow);
		~RayTracer();
	private:
		ThreeDVector* calculate_diffuse_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* diffuse, ThreeDVector* normal);
		ThreeDVector* calculate_color(Surface* surface, vector<Light*> lights, ThreeDVector* normal, ThreeDVector* point_hit, ThreeDVector* view_direction);
		ThreeDVector* calculate_specular_helper(Light* l, ThreeDVector* light_direction, ThreeDVector* specular, ThreeDVector* normal, ThreeDVector* view_direction, long double power_coefficient);
};

#endif