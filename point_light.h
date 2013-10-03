#ifndef POINTLIGHT_H
#define POINTLIGHT_H
#include "light.h"

class PointLight: public Light {
  public:
  	float x, y, z;
  	PointLight(float, float, float, float, float, float);
};

#endif