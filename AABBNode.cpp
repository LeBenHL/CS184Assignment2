#include "AABBNode.h"

struct x_comp{
    inline bool operator()(const pair<Surface*, ThreeDVector*> a, const pair<Surface*, ThreeDVector*> b)
    {   
        return a.second->x < b.second->x;
    }   
};

struct y_comp{
    inline bool operator()(const pair<Surface*, ThreeDVector*> a, const pair<Surface*, ThreeDVector*> b)
    {   
        return a.second->y < b.second->y;
    }   
};

struct z_comp{
    inline bool operator()(const pair<Surface*, ThreeDVector*> a, const pair<Surface*, ThreeDVector*> b)
    {   
        return a.second->z < b.second->z;
    }   
};

AABBNode::AABBNode(vector<Surface*> _surfaces, int axis) {
	surfaces = _surfaces;
	bounds = NULL;
	right = NULL;
	left = NULL;

	if (surfaces.size() > 1) {
		vector<pair<Surface*, ThreeDVector*> > sorted_surfaces;
		for(vector<Surface*>::iterator it = surfaces.begin(); it != surfaces.end(); ++it) {
			Surface* surface = *it;
			Bounds* bounds = surface->bounds->clone();
			this->enclose(bounds);
			double x_min = bounds->x_min;
			double y_min = bounds->y_min;
			double z_min = bounds->z_min;
			ThreeDVector* mins = new ThreeDVector(x_min, y_min, z_min);
			sorted_surfaces.push_back(make_pair(surface, mins));
			delete bounds;
		}
		switch (axis % 3) {
			case 0:
			    sort(sorted_surfaces.begin(), sorted_surfaces.end(), x_comp());
	    		break;
			case 1:
			   sort(sorted_surfaces.begin(), sorted_surfaces.end(), y_comp());
			   break;
			case 2:
			   sort(sorted_surfaces.begin(), sorted_surfaces.end(), z_comp());
			   break;
		}

		vector<pair<Surface*, ThreeDVector*> > left_surface_pairs(sorted_surfaces.begin(), sorted_surfaces.begin() + sorted_surfaces.size()/2),
	               	   right_surface_pairs(sorted_surfaces.begin() + sorted_surfaces.size()/2, sorted_surfaces.end());


	    vector<Surface*> left_surfaces, right_surfaces; 
	    for(vector<pair<Surface*, ThreeDVector*> >::iterator it = left_surface_pairs.begin(); it != left_surface_pairs.end(); ++it) {
	    	pair<Surface*, ThreeDVector*> the_pair = *it;
	    	delete the_pair.second;
	    	left_surfaces.push_back(the_pair.first);
	    }

	    for(vector<pair<Surface*, ThreeDVector*> >::iterator it = right_surface_pairs.begin(); it != right_surface_pairs.end(); ++it) {
	    	pair<Surface*, ThreeDVector*> the_pair = *it;
	    	delete the_pair.second;
	    	right_surfaces.push_back(the_pair.first);
	    }

	    if (left_surfaces.size() > 0) {
	    	this->left = new AABBNode(left_surfaces, axis + 1);
		}

		if (right_surfaces.size() > 0) {
	 	   this->right = new AABBNode(right_surfaces, axis + 1);
	 	}
	} else if (surfaces.size() == 1) {
		bounds = (surfaces[0])->bounds->clone();
	}

}

void AABBNode::enclose(Bounds* bounds) {

	if (this->bounds != NULL) {
		double long x_min = min(bounds->x_min, this->bounds->x_min);
		double long x_max = max(bounds->x_max, this->bounds->x_max);
		double long y_min = min(bounds->y_min, this->bounds->y_min);
		double long y_max = max(bounds->y_max, this->bounds->y_max);
		double long z_min = min(bounds->z_min, this->bounds->z_min);
		double long z_max = max(bounds->z_max, this->bounds->z_max);

		delete this->bounds;
		this->bounds = new Bounds(x_min, x_max, y_min, y_max, z_min, z_max);
	} else {
		this->bounds = bounds->clone();
	}
}

bool AABBNode::is_leaf_node() {
	return (this->right == NULL && this->left == NULL); 
}

vector<Surface*> AABBNode::relevant_surfaces(Ray* ray) {
	vector<Surface*> relevant_surfaces;

	if (this->bounds->hits(ray)) {
		if (this->is_leaf_node()) {
			if (this->surfaces.size() == 1) {
				relevant_surfaces = this->surfaces;
			} else {
				cout << "WTF MODE" << endl;
				exit(1);
			}
		} else {
			vector<Surface*> left_relevant_surfaces = this->left->relevant_surfaces(ray);
			vector<Surface*> right_relevant_surfaces = this->right->relevant_surfaces(ray);
			relevant_surfaces.reserve( left_relevant_surfaces.size() + right_relevant_surfaces.size() );
			relevant_surfaces.insert( relevant_surfaces.end(), left_relevant_surfaces.begin(), left_relevant_surfaces.end() );
			relevant_surfaces.insert( relevant_surfaces.end(), right_relevant_surfaces.begin(), right_relevant_surfaces.end() );		
		}
	} 

	return relevant_surfaces;
}