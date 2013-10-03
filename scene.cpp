#include "scene.h"

Scene::Scene(Camera* _camera, std::vector<Surface*> _surfaces, int _l, int _r, int _b, int _t) {
	image_width = width;
	image_height = height;
	camera = _camera;
	surfaces = _surfaces;
	l = _l;
	r = _r;
	b = _b;
	t = _t;
	image_width = r - l;
	image_height = t - h;
}

Scene::~Scene(){
	delete camera;
}

ThreeDVector* Scene::get_color(int x,int y) {


	return new ThreeDVector(255, 0, 0);
}

ThreeDVector* Scene::get_color_helper(int x, int y, int grid_size) {
	ThreeDVector* color_summation;
	for (int w = 0; w < grid_size; w++) {
		for (int l = 0; l < grid_size; l++) {
			//Generate View Ray, Find what it hits, Compute Color, Add to Color Summation

		}
	}

	ThreeDVector* averaged_color = color_summation->scalar_multiply(1.0 / (grid_size * grid_size));
	delete color_summation;
	return averaged_color;
}