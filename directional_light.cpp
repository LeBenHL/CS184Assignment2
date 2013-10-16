#include "directional_light.h"
#include <iostream>
using namespace std;

DirectionalLight::DirectionalLight(long double _x, long double _y, long double _z, long double r, long double g, long double b) {
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
	return new Ray(position->clone(), this->direction->scalar_multiply(-1), 0, numeric_limits<long double>::infinity());
}

vector<Ray*> DirectionalLight::get_shadow_rays(ThreeDVector* position, int sample_size) {
	vector<Ray*> rays(sample_size, this->get_shadow_ray(position));
	return rays;
}

void DirectionalLight::apply_transformation(Eigen::Matrix4f transformation) {
	Eigen::Vector4f old_direction = Eigen::Vector4f(this->direction->x, this->direction->y, this->direction->z, 0);
	Eigen::Vector4f new_direction = transformation * old_direction;

    delete this->direction;

    this->direction = new ThreeDVector(new_direction[0], new_direction[1], new_direction[2]);
    this->direction->normalize_bang();
}

double long DirectionalLight::get_attenuation_factor(ThreeDVector* position) {
	return 1.0;
}

DirectionalLight::~DirectionalLight(){
	delete direction;
}