#include <math.h> 
#include <vector>
#include <iterator> // For global begin() and end()

#include "sphere.h"

Sphere::Sphere(ThreeDVector* _center, float _radius) {
	center = _center;
	radius = _radius;
}

bool Sphere::hit(Ray* ray, Record* record) {
	//Implementation from Page 77 of the book
	ThreeDVector* e_minus_c = ray->position->vector_subtract(this->center);
	float d_dot_d = ray->direction->dot_product(ray->direction);
	float discriminant = pow((ray->direction->dot_product(e_minus_c)), 2) - d_dot_d * (e_minus_c->dot_product(e_minus_c) - pow(this->radius, 2));
	if (discriminant < 0) {
		// WE DIDN'T HIT IT
		delete e_minus_c;
		return false;
	}
	ThreeDVector* neg_d = ray->direction->scalar_multiply(-1);
	float neg_d_dot_e_minus_c = neg_d->dot_product(e_minus_c);
	float sqrt_discriminant = sqrt(discriminant); 
	float t_hit_pos = (neg_d_dot_e_minus_c + sqrt_discriminant) / d_dot_d; 
	float t_hit_neg =  (neg_d_dot_e_minus_c - sqrt_discriminant) / d_dot_d; 
	
	vector<float> t_hits;
	if (t_hit_pos <= ray->t_max && t_hit_pos >= ray->t_min) {
		t_hits.push_back(t_hit_pos);
	}

	if (t_hit_neg <= ray->t_max && t_hit_neg >= ray->t_min) {
		t_hits.push_back(t_hit_neg);
	}

	if (t_hits.empty()) {
		delete e_minus_c;
		delete neg_d;
		return false;
	}

	float t_hit = *std::min_element(t_hits.begin(), t_hits.end());

	record->t_hit = t_hit;

	delete e_minus_c;
	delete neg_d;
	return true; 

}

ThreeDVector* Sphere::get_normal(ThreeDVector* surface_point){
	ThreeDVector* surface_minus_center = surface_point->vector_subtract(this->center);
	ThreeDVector* normal = surface_minus_center->normalize();
	delete surface_minus_center;
	return normal;
}

Sphere::~Sphere() {
	delete center;
}