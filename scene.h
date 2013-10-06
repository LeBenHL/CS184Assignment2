#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "three_d_vector.h"
#include "ray.h"
#include "camera.h"
#include "surface.h"
#include "sampler.h"
#include "raytracer.h"

using namespace std;

class Scene{
	public:
		Camera* camera;
		std::vector<Surface*> surfaces;
		Sampler* sampler;
		RayTracer* tracer;

		Scene(Camera*, std::vector<Surface*>, int, int);
		~Scene();
		ThreeDVector* get_color(int, int);
		ThreeDVector* get_color_helper(int, int, int);
};


#endif