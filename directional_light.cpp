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

ThreeDVector* DirectionalLight::get_light_direction_from(ThreeDVector* postion) {
	return direction->scalar_multiply(-1);
}

DirectionalLight::~DirectionalLight(){
	delete direction;
}