#include "scene.h"

Scene::Scene(Camera* _camera, std::vector<Surface*> _surfaces, int width, int height) {
	camera = _camera;
	surfaces = _surfaces;

	sampler = new Sampler(width, height);
}

ThreeDVector* Scene::get_color(int x,int y) {

	return this->get_color_helper(x, y, 1);
}

ThreeDVector* Scene::get_color_helper(int x, int y, int grid_size) {
	vector<ThreeDVector*> samples = sampler->get_sample(x, y, grid_size);
	for(vector<ThreeDVector*>::iterator it = samples.begin(); it != samples.end(); ++it) {
    	Ray* view_ray = this->camera->get_view_ray(*it);
	}
	ThreeDVector* color_summation = new ThreeDVector(0, 0, 0);	

	ThreeDVector* averaged_color = color_summation->scalar_multiply(1.0 / (grid_size * grid_size));
	delete color_summation;
	return averaged_color;
}

Scene::~Scene(){
	delete camera;
	delete sampler;
}