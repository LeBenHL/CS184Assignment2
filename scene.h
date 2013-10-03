#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "three_d_vector.h"
#include "ray.h"
#include "camera.h"
#include "surface.h"

class Scene{
	public:
		Camera* camera;
		int image_width;
		int image_height;
		int l;
		int r;
		int t;
		int b;
		std::vector<Surface*> surfaces;
		Scene(Camera*, std::vector<Surface*>, int, int, int, int);
		~Scene();
		ThreeDVector* get_color(int, int);
		ThreeDVector* get_color_helper(int, int, int);
};


#endif