#ifndef BOUNDS_H
#define BOUNDS_H
#include <Eigen/Dense>
#include "three_d_vector.h"
#include "ray.h"

using namespace std;

class Bounds {
  public:
  	//The bounds
    Bounds(long double _x_min, long double _x_max, long double _y_min, long double _y_max, long double _z_min, long double _z_max);
  	long double x_min;
  	long double x_max;
  	long double y_min;
  	long double y_max;
  	long double z_min;
  	long double z_max;
    Bounds* clone();
    bool hits(Ray* ray);
    void apply_transformation(Eigen::Matrix4f transformation);
};

#endif