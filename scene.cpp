#include "scene.h"

Scene::Scene(Camera* _camera, std::vector<Surface*> _surfaces, int width, int height) {
	camera = _camera;
	surfaces = _surfaces;

	sampler = new Sampler(width, height);
}

ThreeDVector* Scene::get_color(int x,int y) {

	return new ThreeDVector(255, 0, 0);
}

ThreeDVector* Scene::get_color_helper(int x, int y, int grid_size) {
	vector<ThreeDVector*> samples = sampler->get_sample(x, y, grid_size);
	ThreeDVector* color_summation;	

	ThreeDVector* averaged_color = color_summation->scalar_multiply(1.0 / (grid_size * grid_size));
	delete color_summation;
	return averaged_color;
}

Scene::~Scene(){
	delete camera;
	delete sampler;
}