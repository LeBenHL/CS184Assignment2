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