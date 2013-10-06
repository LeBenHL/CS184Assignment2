#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <vector>

#include "three_d_vector.h"
#include "ray.h"
#include "surface.h"

using namespace std;

class RayTracer{
	public:
		vector<Surface*> surfaces;
		ThreeDVector* trace(Ray* ray);
		RayTracer(vector<Surface*> surfaces);
		~RayTracer();
};

#endif