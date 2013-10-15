#include "scene.h"

Scene::Scene(Camera* _camera, std::vector<Surface*> _surfaces, std::vector<Light*> _lights, int width, int height, int _recursive_depth, int _grid_size, float focal_length, bool soft_shadow) {
	camera = _camera;
	surfaces = _surfaces;
	lights = _lights;
	recursive_depth = _recursive_depth;
	grid_size = _grid_size;

	camera->focal_length = focal_length;
	sampler = new Sampler(width, height, camera->fov, focal_length);
	tracer = new RayTracer(surfaces, lights, soft_shadow);
}

ThreeDVector* Scene::get_color(int x,int y) {

	return this->get_color_helper(x, y, this->grid_size);
}

ThreeDVector* Scene::get_color_helper(int x, int y, int grid_size) {
	vector<ThreeDVector*> samples = sampler->get_sample(x, y, grid_size);
	ThreeDVector* color_summation = new ThreeDVector(0, 0, 0);	
	for(vector<ThreeDVector*>::iterator it = samples.begin(); it != samples.end(); ++it) {
    	Ray* view_ray = this->camera->get_view_ray(*it);
    	ThreeDVector* color_sample = this->tracer->trace(view_ray, this->recursive_depth, NULL);
    	color_summation->vector_add_bang(color_sample);
    	delete color_sample;
	}

	//Average out colors by number of samples
	color_summation->scalar_multiply_bang(1.0 / (grid_size * grid_size));
	return color_summation;
}

Scene::~Scene(){
	delete camera;
	delete sampler;
	delete tracer;
}