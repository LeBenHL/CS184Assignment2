#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

#include "three_d_vector.h"
#include "ray.h"
#include <Eigen/Dense>

class Light {
  public:
    float red, green, blue;
    virtual ThreeDVector* get_light_direction_from(ThreeDVector* postion) = 0;
    virtual Ray* get_shadow_ray(ThreeDVector* postion) = 0;
    virtual vector<Ray*> get_shadow_rays(ThreeDVector* position, int sample_size) = 0;
    virtual void apply_transformation(Eigen::Matrix4f transformation) = 0;
};

#endif