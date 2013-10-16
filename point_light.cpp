#include "point_light.h"

#include <cmath>

PointLight::PointLight(long double _x, long double _y, long double _z, long double r, long double g, long double b) {
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
	long double mag = direction->magnitude();
	direction->normalize_bang();
	return new Ray(position->clone(), direction, 0, mag);
}

vector<Ray*> PointLight::get_shadow_rays(ThreeDVector* position, int sample_size) {
	// SOFT SHADOWING TAKEN FROM SJSU
	vector<Ray*> rays;
	for (int i=0; i<sample_size; i++) {
		extern long double PI;
		long double u = ((long double) rand()) / RAND_MAX;
		long double v = ((long double) rand()) / RAND_MAX;

		long double q = 2 * PI * u;
		long double f = acos(2 * v - 1);

		long double delta_x = this->radius * cos(q) * sin(f);
		long double delta_y = this->radius * sin(q) * sin(f);
		long double delta_z = this->radius * cos(f);

		ThreeDVector* delta = new ThreeDVector(delta_x, delta_y, delta_z);
		ThreeDVector* light_position = this->position->vector_add(delta);

		ThreeDVector* direction = light_position->vector_subtract(position);
		long double mag = direction->magnitude();
		direction->normalize_bang();
		Ray* ray = new Ray(position->clone(), direction, 0, mag);
		rays.push_back(ray);

		delete delta;
		delete light_position;
	}
	return rays;
}

void PointLight::apply_transformation(Eigen::Matrix4f transformation) {
	Eigen::Vector4f old_pos = Eigen::Vector4f(this->position->x, this->position->y, this->position->z, 1);
	Eigen::Vector4f new_pos = transformation * old_pos;

    delete this->position;

    this->position = new ThreeDVector(new_pos[0], new_pos[1], new_pos[2]);
}

PointLight::~PointLight(){
	delete position;
}