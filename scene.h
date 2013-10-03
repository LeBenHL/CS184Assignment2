#ifndef SCENE_H
#define SCENE_H

#include "three_d_vector.h"
#include "ray.h"

class Scene{
	public:
		ThreeDVector* LL;
		ThreeDVector* LR;
		ThreeDVector* UL;
		ThreeDVector* UR;
		Camera* camera;
		int image_width;
		int image_height;
		Scene(Camera*, int, int);
		~Scene();
		ThreeDVector* get_color(int, int);
};


#endif