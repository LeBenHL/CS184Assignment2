#ifndef LIGHT_H
#define LIGHT_H

#include <vector>

#include "three_d_vector.h"
#include "ray.h"

class Light {
  public:
    float red, green, blue;
    float shadow_t_min;
    virtual ThreeDVector* get_light_direction_from(ThreeDVector* postion) = 0;
    virtual Ray* get_shadow_ray(ThreeDVector* postion) = 0;
    virtual vector<ThreeDVector*> get_light_samples(ThreeDVector* position) = 0;
};

#endif