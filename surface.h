#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"
#include "three_d_vector.h"

class Surface {
	public:
		ThreeDVector* diffuse;
		ThreeDVector* specular;
		ThreeDVector* emission;
		float power_coefficient;

		virtual bool hit(Ray* ray, Record* record) = 0;
		virtual ThreeDVector* get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector) = 0;
};

#endif