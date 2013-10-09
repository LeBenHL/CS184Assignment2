#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H
#include "light.h"
#include "three_d_vector.h"

class DirectionalLight: public Light {
  public:
  	ThreeDVector* direction;
  	ThreeDVector* get_light_direction_from(ThreeDVector* postion);
  	Ray* get_shadow_ray(ThreeDVector* postion);
  	vector<Ray*> get_shadow_rays(ThreeDVector* position, int sample_size);
  	DirectionalLight(float, float, float, float, float, float);
  	~DirectionalLight();
};

#endif