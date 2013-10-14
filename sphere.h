#ifndef SPHERE_H
#define SPHERE_H
#include "surface.h"
#include "three_d_vector.h"

using namespace std;

class Sphere: public Surface {
  public:
  	ThreeDVector* center;
  	float radius;
  	Sphere(ThreeDVector* center, float radius);
  	bool hit(Ray* _ray, Record* record);
  	ThreeDVector* get_normal(ThreeDVector* surface_point, ThreeDVector* view_vector);
  	void apply_transformation(Eigen::Matrix4f transformation);
  	~Sphere();
};

#endif