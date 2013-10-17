#ifndef SURFACE_H
#define SURFACE_H

#include "ray.h"
#include "three_d_vector.h"
#include <Eigen/Dense>
#include "Bounds.h"

class Surface {
	public:
		ThreeDVector* diffuse;
		ThreeDVector* specular;
		ThreeDVector* emission;
		Eigen::Matrix4f transformation;
		Eigen::Matrix4f inverse;
		Eigen::Matrix4f inverse_transpose;
		Bounds* bounds;
		long double power_coefficient;
		float transparency;

		virtual bool hit(Ray* ray, Record* record) = 0;
		virtual ThreeDVector* get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector) = 0;
		virtual void apply_transformation(Eigen::Matrix4f transformation) = 0;
		virtual void get_bounds() = 0;
};

#endif