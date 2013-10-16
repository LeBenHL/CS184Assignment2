#include "Bounds.h"

Bounds::Bounds(long double _x_min, long double _x_max, long double _y_min, long double _y_max, long double _z_min, long double _z_max) {
	x_min = _x_min;
	x_max = _x_max;
	y_min = _y_min;
	y_max = _y_max;
	z_min = _z_min;
	z_max = _z_max;
}

Bounds* Bounds::clone() {
	return new Bounds(this->x_min, this->x_max, this->y_min, this->y_max, this->z_min, this->z_max);
}

void Bounds::apply_transformation(Eigen::Matrix4f transformation) {
	ThreeDVector* v1 = new ThreeDVector(this->x_min, this->y_min, this->z_min);
	ThreeDVector* v2 = new ThreeDVector(this->x_min, this->y_min, this->z_max);
	ThreeDVector* v3 = new ThreeDVector(this->x_min, this->y_max, this->z_min);
	ThreeDVector* v4 = new ThreeDVector(this->x_min, this->y_max, this->z_max);
	ThreeDVector* v5 = new ThreeDVector(this->x_max, this->y_min, this->z_min);
	ThreeDVector* v6 = new ThreeDVector(this->x_max, this->y_min, this->z_max);
	ThreeDVector* v7 = new ThreeDVector(this->x_max, this->y_max, this->z_min);
	ThreeDVector* v8 = new ThreeDVector(this->x_max, this->y_max, this->z_max);

	v1->transform_bang(transformation, true);
	v2->transform_bang(transformation, true);
	v3->transform_bang(transformation, true);
	v4->transform_bang(transformation, true);
	v5->transform_bang(transformation, true);
	v6->transform_bang(transformation, true);
	v7->transform_bang(transformation, true);
	v8->transform_bang(transformation, true);

	this->x_min = min(v1->x, min(v2->x, min(v3->x, min(v4->x, min(v5->x, min(v6->x, min(v7->x, v8->x)))))));
	this->y_min = min(v1->y, min(v2->y, min(v3->y, min(v4->y, min(v5->y, min(v6->y, min(v7->y, v8->y)))))));
	this->z_min = min(v1->z, min(v2->z, min(v3->z, min(v4->z, min(v5->z, min(v6->z, min(v7->z, v8->z)))))));

	this->x_max = max(v1->x, max(v2->x, max(v3->x, max(v4->x, max(v5->x, max(v6->x, max(v7->x, v8->x)))))));
	this->y_max = max(v1->y, max(v2->y, max(v3->y, max(v4->y, max(v5->y, max(v6->y, max(v7->y, v8->y)))))));
	this->z_max = max(v1->z, max(v2->z, max(v3->z, max(v4->z, max(v5->z, max(v6->z, max(v7->z, v8->z)))))));

}

// Optimized method from http://people.csail.mit.edu/amy/papers/box-jgt.pdf
bool Bounds::hits(Ray* ray) {
	long double tmin, tmax, tymin, tymax, tzmin, tzmax;
	if (ray->inv_direction->x < 0) {
		tmin = (this->x_max - ray->position->x) * ray->inv_direction->x;
		tmax = (this->x_min - ray->position->x) * ray->inv_direction->x;
	} else {
		tmin = (this->x_min - ray->position->x) * ray->inv_direction->x;
		tmax = (this->x_max - ray->position->x) * ray->inv_direction->x;
	}

	if (ray->inv_direction->y < 0) {
		tymin = (this->y_max - ray->position->y) * ray->inv_direction->y;
		tymax = (this->y_min - ray->position->y) * ray->inv_direction->y;
	} else {
		tymin = (this->y_min - ray->position->y) * ray->inv_direction->y;
		tymax = (this->y_max - ray->position->y) * ray->inv_direction->y;
	}

	if ( (tmin > tymax) || (tymin > tmax) ) {
		return false;
	}
	if (tymin > tmin) {
		tmin = tymin;
	}
	if (tymax < tmax) {
		tmax = tymax;
	}

	if (ray->inv_direction->z < 0) {
		tzmin = (this->z_max - ray->position->z) * ray->inv_direction->z;
		tzmax = (this->z_min - ray->position->z) * ray->inv_direction->z;
	} else {
		tzmin = (this->z_min - ray->position->z) * ray->inv_direction->z;
		tzmax = (this->z_max - ray->position->z) * ray->inv_direction->z;
	}
	if ( (tmin > tzmax) || (tzmin > tmax) ) {
		return false;
	}
	if (tzmin > tmin) {
		tmin = tzmin;
	}
	if (tzmax < tmax) {
		tmax = tzmax;
	}
	return ( (tmin < ray->t_max) && (tmax > ray->t_min) );
}

