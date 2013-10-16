#ifndef AABBNODE_H
#define AABBNODE_H
#include <vector>
#include "Bounds.h"
#include "surface.h"
#include "three_d_vector.h"
#include "ray.h"

using namespace std;

class AABBNode {
  public:
  	AABBNode* left;
  	AABBNode* right;

  	//The bounds
  	Bounds* bounds;

  	// Surfaces in this bounding box
  	vector<Surface*> surfaces;
  	AABBNode(vector<Surface*> surfaces, int axis);
  	vector<Surface*> relevant_surfaces(Ray* ray);
  	bool is_leaf_node();
  private:
  	void enclose(Bounds* bounds);
};

#endif