#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"

class Surface {
	public:
		virtual bool hit(Ray* ray, Record* record) = 0;
};

#endif