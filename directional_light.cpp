#include "directional_light.h"
#include <iostream>
using namespace std;

DirectionalLight::DirectionalLight(float _x, float _y, float _z, float r, float g, float b) {
	direction = new ThreeDVector(_x, _y, _z);
	//cout << "BEFORE: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
	direction->normalize_bang();
	//cout << "AFTER: " << direction->x << ", " << direction->y << ", " << direction->z << endl;
	red = r;
	green = g;
	blue = b;
}

ThreeDVector* DirectionalLight::get_light_direction_from(ThreeDVector* position) {
	return this->direction->scalar_multiply(-1);
}

Ray* DirectionalLight::get_shadow_ray(ThreeDVector* position) {
	return new Ray(position->clone(), this->direction->scalar_multiply(-1), 0, numeric_limits<float>::infinity());
}

vector<Ray*> DirectionalLight::get_shadow_rays(ThreeDVector* position, int sample_size) {
	vector<Ray*> rays(sample_size, this->get_shadow_ray(position));
	return rays;
}

DirectionalLight::~DirectionalLight(){
	delete direction;
}