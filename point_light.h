#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class PointLight: public Light {
  public:
  	ThreeDVector* position;
  	PointLight(float, float, float, float, float, float);
  	~PointLight();
  	ThreeDVector* get_light_direction_from(ThreeDVector* postion);
};

#endif