#include "point_light.h"

PointLight::PointLight(float _x, float _y, float _z, float r, float g, float b) {
	position = new ThreeDVector(_x, _y, _z);
	red = r;
	green = g;
	blue = b;
}

ThreeDVector* PointLight::get_light_direction_from(ThreeDVector* postion) {
	ThreeDVector* direction = this->position->vector_subtract(position);
	direction->normalize_bang();
	return direction;
}

PointLight::~PointLight(){
	delete position;
}