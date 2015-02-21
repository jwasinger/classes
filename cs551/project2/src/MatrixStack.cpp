#include "MatrixStack.h"

void MatrixStack::push_mat(Matrix m)
{
	this->mat_vec.push_back(m);
}

void MatrixStack::pop_mat(void)
{
	this->mat_vec.pop_back();
}

Matrix MatrixStack::calc_val(void)
{
	Matrix output;

	for (int i = 0; i < this->mat_vec.size(); i++)
	{
		output = output * this->mat_vec[i];
	}

	//output = output * this->projection;
	
	return output;
}

Matrix &MatrixStack::get_top(void)
{
	return this->mat_vec[mat_vec.size() - 1];
}

MatrixStack::MatrixStack()
{
	this->mat_vec = std::vector<Matrix>(0);
}

MatrixStack::~MatrixStack()
{}