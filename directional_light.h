#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class DirectionalLight: public Light {
  public:
  	ThreeDVector* direction;
  	DirectionalLight(float, float, float, float, float, float);
  	~DirectionalLight();
};

#endif