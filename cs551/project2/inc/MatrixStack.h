#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include "Matrix.h"
#include <vector>

enum PROJ_TYPE
{
	PROJ_PERSPECTIVE,
	PROJ_ORTHO
};

using namespace A2Math;

class MatrixStack
{
private:
	Matrix projection;
	std::vector<Matrix> mat_vec;

public:
	PROJ_TYPE proj_type;

	MatrixStack();
	~MatrixStack();

	void set_proj(Matrix m) { this->projection = m;}
	Matrix get_proj(void) const { return this->projection; }
	void push_mat(Matrix m);
	void pop_mat(void);
	Matrix calc_val(void);
	Matrix &get_top(void);
};
#endif