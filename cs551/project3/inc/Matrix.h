#ifndef MATRIX_H
#define MATRIX_H

#include "Vector.h"
#include <math.h>

class Matrix
{
private:
	double m00, m10, m20, m30;
	double m01, m11, m21, m31;
	double m02, m12, m22, m32;
	double m03, m13, m23, m33;

public:
	Matrix();
	Matrix operator* (const Matrix &mat2);
	Vector4 operator* (const Vector4 &vec);

	void MulWithW(const Vector4 &v, Vector4 &out_v, double &out_w);

	//bool operator==(const DXMatrix &dx_mat);

	static Matrix CreateRotationAxisAngle(const Vector4 &axis, double angle); 
	static Matrix CreateTranslation(const Vector4 &vec);
	static Matrix CreateScale(const Vector4 &vec);
	static Matrix CreateOrthographicLH(double left, double right, double bottom, double top, double near, double far);

	static Matrix CreatePerspectiveFOV(double fov, double aspect, double near, double far);

	static Matrix CreateLookAt(Vector4 target, Vector4 eye, Vector4 up);
	
};

#endif
