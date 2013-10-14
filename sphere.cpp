#include <math.h> 
#include <vector>
#include <iterator> // For global begin() and end()

#include "sphere.h"

Sphere::Sphere(ThreeDVector* _center, float _radius) {
	center = _center;
	radius = _radius;
}

bool Sphere::hit(Ray* _ray, Record* record) {
	bool is_identity = this->transformation == Eigen::Matrix4f::Identity();
	Ray* ray;
	//Transform ray
	if (!is_identity) {
		Eigen::Vector4f old_position = Eigen::Vector4f(_ray->position->x, _ray->position->y, _ray->position->z, 0);
		Eigen::Vector4f old_direction = Eigen::Vector4f(_ray->direction->x, _ray->direction->y, _ray->direction->z, 1);
		Eigen::Vector4f m_inverse_times_position = this->inverse * old_position;
		Eigen::Vector4f m_inverse_times_direction = this->inverse * old_direction;
		ThreeDVector* new_position = new ThreeDVector(m_inverse_times_position[0], m_inverse_times_position[1], m_inverse_times_position[2]);
		ThreeDVector* new_direction = new ThreeDVector(m_inverse_times_direction[0], m_inverse_times_direction[1], m_inverse_times_direction[2]);
		ray = new Ray(new_position, new_direction, _ray->t_min, _ray->t_max);
		cout << this->transformation << endl;
		cout << this->inverse << endl;
		cout << ray->repr() << endl;
		cout << _ray->repr() << endl << endl;
	} else {
		ray = _ray;
	}

	//Implementation from Page 77 of the book
	ThreeDVector* e_minus_c = ray->position->vector_subtract(this->center);
	float d_dot_d = ray->direction->dot_product(ray->direction);
	float discriminant = pow((ray->direction->dot_product(e_minus_c)), 2) - d_dot_d * (e_minus_c->dot_product(e_minus_c) - pow(this->radius, 2));
	if (discriminant < 0) {
		// WE DIDN'T HIT IT
		delete e_minus_c;

		if (!is_identity) {
			delete ray;
		}
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
		if (!is_identity) {
			delete ray;
		}
		return false;
	}

	float t_hit = *std::min_element(t_hits.begin(), t_hits.end());

	record->t_hit = t_hit;

	ThreeDVector* point_hit = ray->point_at(t_hit);

	if (!is_identity) {
		Eigen::Vector4f untransformed_pos = Eigen::Vector4f(point_hit->x, point_hit->y, point_hit->z, 0);
		Eigen::Vector4f point_hit_4f = this->inverse_transpose * untransformed_pos;
		record->position_hit = new ThreeDVector(point_hit_4f[0], point_hit_4f[1], point_hit_4f[2]);
		record->untransformed_position_hit = point_hit;	
	} else {
		record->position_hit = point_hit;
		record->untransformed_position_hit = point_hit;
	}

	delete e_minus_c;
	delete neg_d;
	if (!is_identity) {
		delete ray;
	}
	cout << "HEY" << endl;
	return true; 
}

ThreeDVector* Sphere::get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector){
	ThreeDVector* surface_minus_center = surface_point->vector_subtract(this->center);
	ThreeDVector* normal = surface_minus_center->normalize();
	delete surface_minus_center;
	Eigen::Vector4f untransformed_normal = Eigen::Vector4f(normal->x, normal->y, normal->z, 0);
	delete normal;
	Eigen::Vector4f transformed_normal = this->inverse_transpose * untransformed_normal;
	ThreeDVector* new_normal = new ThreeDVector(transformed_normal[0], transformed_normal[1], transformed_normal[2]);
	return new_normal;
}

Sphere::~Sphere() {
	delete center;
}