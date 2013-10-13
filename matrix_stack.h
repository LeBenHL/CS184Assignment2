#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <stack>
#include <Eigen/Dense>

using namespace std;
using namespace Eigen;

class MatrixStack{
	stack<Matrix4f*> mst;
	MatrixStack();
	void push();
	Matrix4f pop();
};

#endif