#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"

class Surface {
	public:
		virtual bool hit(Ray*, float t0, float t1, float& record);
};

#endif