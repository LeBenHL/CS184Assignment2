#include "scene.h"

Scene::Scene(Camera* _camera, int width, int height) {
	image_width = width;
	image_height = height;
	camera = _camera;

	LL = new ThreeDVector(-1, -1, 0);
	LR = new ThreeDVector(1, -1, 0);
	UL = new ThreeDVector(-1, 1, 0);
	UR = new ThreeDVector(1, 1, 0);
}

Scene::~Scene(){
	delete LL;
	delete LR;
	delete UL;
	delete UR;
	delete camera;
}

ThreeDVector* Scene::get_color(int x,int y) {


	return new ThreeDVector(255, 0, 0);
}

ThreeDVector Scene::get_color_helper(int x, int y, int grid_size) {
	ThreeDVector* color_summation;
	for (int w = 0; w < grid_size; w++) {
		for (int l = 0; l < grid_size; l++) {
			//Generate View Ray, Compute Color, Add to Color Summation

		}
	}

	ThreeDVector* averaged_color = color_summation->scalar_multiply(1.0 / (grid_size * grid_size))
	delete color_summation;
	return averaged_color;
}