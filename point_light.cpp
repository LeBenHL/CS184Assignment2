#include "point_light.h"

#include <cmath>

PointLight::PointLight(float _x, float _y, float _z, float r, float g, float b) {
	position = new ThreeDVector(_x, _y, _z);
	red = r;
	green = g;
	blue = b;

	radius = 0.1;
}

ThreeDVector* PointLight::get_light_direction_from(ThreeDVector* position) {
	ThreeDVector* direction = this->position->vector_subtract(position);
	direction->normalize_bang();
	return direction;
}

Ray* PointLight::get_shadow_ray(ThreeDVector* position) {
	ThreeDVector* direction = this->position->vector_subtract(position);
	float mag = direction->magnitude();
	direction->normalize_bang();
	return new Ray(position->clone(), direction, 0, mag);
}

vector<Ray*> PointLight::get_shadow_rays(ThreeDVector* position, int sample_size) {
	// SOFT SHADOWING TAKEN FROM SJSU
	vector<Ray*> rays;
	for (int i=0; i<sample_size; i++) {
		extern float PI;
		float u = float(rand()) / RAND_MAX;
		float v = float(rand()) / RAND_MAX;

		float q = 2 * PI * u;
		float f = acos(2 * v - 1);

		float delta_x = this->radius * cos(q) * sin(f);
		float delta_y = this->radius * sin(q) * sin(f);
		float delta_z = this->radius * cos(f);

		ThreeDVector* delta = new ThreeDVector(delta_x, delta_y, delta_z);
		ThreeDVector* light_position = this->position->vector_add(delta);

		ThreeDVector* direction = light_position->vector_subtract(position);
		float mag = direction->magnitude();
		direction->normalize_bang();
		Ray* ray = new Ray(position->clone(), direction, 0, mag);
		rays.push_back(ray);

		delete delta;
		delete light_position;
	}
	return rays;
}

PointLight::~PointLight(){
	delete position;
}