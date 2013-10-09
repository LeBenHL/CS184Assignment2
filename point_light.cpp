#include "point_light.h"

PointLight::PointLight(float _x, float _y, float _z, float r, float g, float b) {
	position = new ThreeDVector(_x, _y, _z);
	red = r;
	green = g;
	blue = b;

	shadow_t_min = 0.01;
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
	return new Ray(position->clone(), direction, this->shadow_t_min, mag);
}

PointLight::~PointLight(){
	delete position;
}