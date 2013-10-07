#ifndef LIGHT_H
#define LIGHT_H

#include "three_d_vector.h"

class Light {
  public:
    float red, green, blue;
    virtual ThreeDVector* get_light_direction_from(ThreeDVector* postion) = 0;
};

#endif