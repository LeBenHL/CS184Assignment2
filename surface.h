#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"
#include "three_d_vector.h"

class Surface {
	public:
		ThreeDVector* diffuse;
		ThreeDVector* specular;
		float power_coefficient;

		virtual bool hit(Ray* ray, Record* record) = 0;
		virtual ThreeDVector* get_normal(ThreeDVector* surface_point) = 0;
};

#endif