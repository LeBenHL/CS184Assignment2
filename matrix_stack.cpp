#include matrix_stack.h

MatrixStack::MatrixStack(){
	Matrix4f id = Matrix4f::Identity();
	mst.push(&id);
}

void MatrixStack::push(){
	Matrix4f id = Matrix4f::Identity();
	mst.push(&id);
}

void MatrixStack::pop(){
	mst.pop();
}