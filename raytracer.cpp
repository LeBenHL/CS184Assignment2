#include "raytracer.h"

RayTracer::RayTracer(vector<Surface*> _surfaces) {
	surfaces = _surfaces;
}

ThreeDVector* RayTracer::trace(Ray* ray) {
	bool hit = false;
	Surface* first_hit;
	Record* record = new Record();
	for(vector<Surface*>::iterator it = this->surfaces.begin(); it != this->surfaces.end(); ++it) {
		Surface* surface = *it;
    	if (surface->hit(ray, record)) {
    		hit = true;
    		first_hit = surface;
    		ray->t_max = record->t_hit;
    	} 
	}

	if (hit) {
		return new ThreeDVector(0, 0, 255);
	} else {
		//Return background color
		return new ThreeDVector(0, 0, 0);
	}
}