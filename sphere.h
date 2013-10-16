#ifndef SPHERE_H
#define SPHERE_H
#include "surface.h"
#include "three_d_vector.h"

using namespace std;

class Sphere: public Surface {
  public:
  	ThreeDVector* center;
  	long double radius;
  	Sphere(ThreeDVector* center, long double radius);
  	bool hit(Ray* _ray, Record* record);
  	ThreeDVector* get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector);
  	void apply_transformation(Eigen::Matrix4f transformation);
  	Bounds* get_bounds();
  	~Sphere();
};

#endif