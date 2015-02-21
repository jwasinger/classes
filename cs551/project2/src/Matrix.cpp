#include "Matrix.h"

namespace A2Math
{
	static double cot(double val)
	{
		return 1.0 / (tan(val));
	}

	Matrix::Matrix()
	{
		m00 = 1.0;
		m10 = 0.0;
		m20 = 0.0;
		m30 = 0.0;

		m01 = 0.0;
		m11 = 1.0;
		m21 = 0.0;
		m31 = 0.0;

		m02 = 0.0;
		m12 = 0.0;
		m22 = 1.0;
		m32 = 0.0;

		m03 = 0.0;
		m13 = 0.0;
		m23 = 0.0;
		m33 = 1.0;
	}

	bool Matrix::matrix_tests(void)
	{
		SimpleMath::Matrix dx_mat;
		Matrix my_mat;

		if (dx_mat != my_mat)
		{
			printf("identity matrices not equal...\n");
			return false;
		}
		double pi = 3.14159;
		dx_mat = SimpleMath::Matrix::CreateFromAxisAngle(SimpleMath::Vector3(0, 0, 1), pi/2.0);
		my_mat = Matrix::CreateRotationAxisAngle(Vector4(0.0, 0.0, 1.0, 1.0), pi);

		Vector4 x, y, z, origin;
		x = Vector4(1.0, 0.0, 0.0, 1.0);
		y = Vector4(0.0, 1.0, 0.0, 1.0);
		z = Vector4(0.0, 0.0, 1.0, 1.0);
		origin = Vector4(0.0, 0.0, 0.0, 1.0);

		dx_mat = SimpleMath::Matrix::CreateLookAt(origin, z, y);
		my_mat = Matrix::CreateLookAt(x, origin, y);

		if (dx_mat != my_mat)
		{
			return false;
		}

		my_mat = Matrix::CreatePerspectiveFOV(60.0 * (pi / 180.0), 1.0, 0.0, 10000.0);
		dx_mat = SimpleMath::Matrix::CreatePerspectiveFieldOfView(60.0 * (pi / 180.0), 1.0, 0.0, 10000.0);
		//false ^^


		my_mat = Matrix::CreateOrthographicLH(0.0, 1.0, 0.0, 1.0, 0.1, 1.0);
		dx_mat = SimpleMath::Matrix::CreateOrthographicOffCenter(0.0, 1.0, 0.0, 1.0, 0.1, 1.0);
		//false ^^

		my_mat = Matrix::CreateTranslation(Vector4(100, -100, 100, 1.0));
		dx_mat = SimpleMath::Matrix::CreateTranslation(SimpleMath::Vector3(100, -100, 100));

		if (dx_mat != my_mat)
		{
			return false;
		}

		my_mat = Matrix::CreateScale(Vector4(10.0, 0.1, 1.0, 1.0));
		dx_mat = SimpleMath::Matrix::CreateScale(10.0f, 0.1f, 1.0f);

		if (dx_mat != my_mat)
		{
			return false;
		}
		return true;
	}

	/*bool Matrix::operator==(const DXMatrix &dx_mat)
	{
	DXMatrix conv = DXMatrix()
	}*/

	Matrix::operator SimpleMath::Matrix()
	{
		SimpleMath::Matrix mat = SimpleMath::Matrix(
			m00, m10, m20, m30,
			m01, m11, m21, m31,
			m02, m12, m22, m32,
			m03, m13, m23, m33);
		return mat;
	}

	Matrix Matrix::operator* (const Matrix &m2)
	{
		Matrix output;

		output.m00 = m00 * m2.m00 + m10 * m2.m01 + m20 * m2.m02 + m30 * m2.m03;
		output.m10 = m00 * m2.m10 + m10 * m2.m11 + m20 * m2.m12 + m30 * m2.m13;
		output.m20 = m00 * m2.m20 + m10 * m2.m21 + m20 * m2.m22 + m30 * m2.m23;
		output.m30 = m00 * m2.m30 + m10 * m2.m31 + m20 * m2.m32 + m30 * m2.m33;

		output.m01 = m01 * m2.m00 + m11 * m2.m01 + m21 * m2.m02 + m31 * m2.m03;
		output.m11 = m01 * m2.m10 + m11 * m2.m11 + m21 * m2.m12 + m31 * m2.m13;
		output.m21 = m01 * m2.m20 + m11 * m2.m21 + m21 * m2.m22 + m31 * m2.m23;
		output.m31 = m01 * m2.m30 + m11 * m2.m31 + m21 * m2.m32 + m31 * m2.m33;

		output.m02 = m02 * m2.m00 + m12 * m2.m01 + m22 * m2.m02 + m32 * m2.m03;
		output.m12 = m02 * m2.m10 + m12 * m2.m11 + m22 * m2.m12 + m32 * m2.m13;
		output.m22 = m02 * m2.m20 + m12 * m2.m21 + m22 * m2.m22 + m32 * m2.m23;
		output.m32 = m02 * m2.m30 + m12 * m2.m31 + m22 * m2.m32 + m32 * m2.m33;

		output.m03 = m03 * m2.m00 + m13 * m2.m01 + m23 * m2.m02 + m33 * m2.m03;
		output.m13 = m03 * m2.m10 + m13 * m2.m11 + m23 * m2.m12 + m33 * m2.m13;
		output.m23 = m03 * m2.m20 + m13 * m2.m21 + m23 * m2.m22 + m33 * m2.m23;
		output.m33 = m03 * m2.m30 + m13 * m2.m31 + m23 * m2.m32 + m33 * m2.m33;

		return output;
	}

	Vector4 Matrix::operator* (const Vector4 &v)
	{
		double x, y, z, w;
		x = m00 * v.X + m10 * v.Y + m20 * v.Z + v.W * m30;
		y = m01 * v.X + m11 * v.Y + m21 * v.Z + m31 * v.W;
		z = m02 * v.X + m12 * v.Y + m22 * v.Z + m32 * v.W;
		w = m03 * v.X + m13 * v.Y + m23 * v.Z + m33 * v.W;
		return Vector4(x, y, z, w);
	}

	Matrix Matrix::CreateOrthographicLH(double left, double right, double bottom, double top, double near, double far)
	{
		Matrix m;
		m.m00 = 2.0 / (right - left);
		m.m10 = 0.0;
		m.m20 = 0.0;
		m.m30 = -((right + left) / (right - left));

		m.m01 = 0.0;
		m.m11 = 2.0 / (top - bottom);
		m.m21 = 0.0;
		m.m31 = -((top + bottom) / (top - bottom));

		m.m02 = 0.0;
		m.m12 = 0.0;
		m.m22 = 1.0 / (far - near);
		m.m32 = -(near) / (far - near);

		m.m03 = 0.0;
		m.m13 = 0.0;
		m.m23 = 0.0;
		m.m33 = 1.0;

		return m;
	}

	Matrix Matrix::CreatePerspectiveFOV(double fov, double aspect, double near, double far)
	{
		near = -near;
		far = -far;

		Matrix m;
		m.m00 = (1.0 / aspect)*cot(fov / 2.0);
		m.m10 = 0.0;
		m.m20 = 0.0;
		m.m30 = 0.0;

		m.m01 = 0.0;
		m.m11 = cot(fov / 2.0);
		m.m21 = 0.0;
		m.m31 = 0.0;

		m.m02 = 0.0;
		m.m12 = 0.0;
		/*
		m.m22 = far / (far - near);
		m.m32 = -(far * near) / (far - near);
		*/
		m.m22 = (-far - near) / (far - near);
		m.m32 = (2 * near * far) / (far - near);

		m.m03 = 0.0;
		m.m13 = 0.0;
		m.m23 = 1.0;
		m.m33 = 0.0;

		/*
		Matrix m_ortho = Matrix::CreateOrthographicLH(0.0, 1.0, 0.0, 1.0, -1.0, 1.0);
		m = m * m_ortho;
		*/

		Vector4 v1 = Vector4(0.5, 0.5, -1.0, 1.0);
		Vector4 v2 = Vector4(0.5, 0.5, -100.0, 1.0);
		Vector4 v1_t, v2_t;

		v1_t = m * v1;
		v2_t = m * v2;

		return m;

		/*Matrix m;

		m.m00 = 2 * near;
		m.m10 = 0.0;
		m.m20 = 0.0;
		m.m30 = 0.0;

		m.m01 = 0.0;
		m.m11 = 0.0;
		m.m21 = 0.0;
		m.m31 = 0.0;

		m.m02 = 0.0;
		m.m12 = 0.0;
		m.m22 = -1.0;
		m.m32 = 0.0;*/
	}

	Matrix Matrix::CreateRotationAxisAngle(const Vector4 &a, double angle)
	{
		//angle = 2 * 3.14159 - 2 * angle;
		Vector4 axis = a;
		axis.Normalize();
		double c = cos(angle);
		double s = sin(angle);
		double C = 1 - c;

		Matrix m;
		m.m00 = axis.X * axis.X * C + c;
		m.m10 = axis.X * axis.Y * C - axis.Z * s;
		m.m20 = axis.X * axis.Z * C + axis.Y * s;
		m.m30 = 0.0;

		m.m01 = axis.Y * axis.X * C + axis.Z * s;
		m.m11 = axis.Y * axis.Y * C + c;
		m.m21 = axis.Y * axis.Z * C - axis.X * s;
		m.m31 = 0.0;

		m.m02 = axis.Z * axis.X * C - axis.Y * s;
		m.m12 = axis.Z * axis.Y * C + axis.X * s;
		m.m22 = axis.Z * axis.Z * C + c;
		m.m23 = 0.0;
		
		return m;
		//original ^^

		/*
		m.m00 = axis.X() * axis.X() * C + c;
		m.m20 = axis.X() * axis.Z() * C + axis.Y() * s;
		m.m10 = axis.X() * axis.Y() * C - axis.Z() * s;
		m.m30 = 0.0;

		m.m21 = axis.Y() * axis.Z() * C - axis.X() * s;
		m.m11 = axis.Y() * axis.Y() * C + c;
		m.m01 = axis.Y() * axis.X() * C + axis.Z() * s;
		m.m31 = 0.0;

		m.m12 = axis.Z() * axis.Y() * C + axis.X() * s;
		m.m02 = axis.Z() * axis.X() * C - axis.Y() * s;
		m.m22 = axis.Z() * axis.Z() * C + c;
		m.m23 = 0.0;
		*/
		//modified ^^

		/*Matrix m;
		double c = cos(angle);
		double s = sin(angle);
		double d = 1 - c;
		Vector4 su = Vector4(s*axis.X, s*axis.Y, s*axis.Z, 1.0);
		Vector4 du = Vector4(d*axis.X, d*axis.Y, d*axis.Z, 1.0);
		
		m.m00 = du.X*axis.X + c;
		m.m10 = du.X*axis.Y + su.Z;
		m.m20 = du.X*axis.Z - su.Y;
		m.m30 = 0.0;

		m.m01 = du.Y*axis.X - su.Z;
		m.m11 = du.Y*axis.Y + c;
		m.m21 = du.Y*axis.Z + su.X;
		m.m31 = 0.0;

		m.m02 = du.Z*axis.X + su.Y;
		m.m12 = du.Z*axis.Y - su.X;
		m.m22 = du.Z*axis.Z + c;
		m.m32 = 0.0;

		m.m03 = 0.0;
		m.m13 = 0.0;
		m.m23 = 0.0;
		m.m33 = 1.0;

		return m;*/
	}

	void Matrix::MulWithW(const Vector4 &v, Vector4 &out_v, double &out_w)
	{
		out_v = (*this) * v;
		out_w = m03 * v.X + m13 * v.Y + m23 * v.Z + m33;
	}

	Matrix Matrix::CreateLookAt(Vector4 target, Vector4 eye, Vector4 up)
	{
		target.Normalize();
		eye.Normalize();
		up.Normalize();
		Vector4 right = target.Cross(up);

		Matrix m;
		m.m00 = right.X;
		m.m10 = up.X;
		m.m20 = target.X;
		m.m30 = 0.0;

		m.m01 = right.Y;
		m.m11 = up.Y;
		m.m21 = target.Y;
		m.m31 = 0.0;

		m.m02 = right.Z;
		m.m12 = up.Z;
		m.m22 = target.Z;
		m.m32 = 0.0;

		m.m03 = -right.Dot(eye);
		m.m13 = -up.Dot(eye);
		m.m23 = -target.Dot(eye);
		m.m33 = 1.0;

		return m;
	}

	Matrix Matrix::CreateScale(const Vector4 &v)
	{
		Matrix m;
		m.m00 = v.X;
		m.m11 = v.Y;
		m.m22 = v.Z;
		return m;
	}

	Matrix Matrix::CreateTranslation(const Vector4 &vec)
	{
		Matrix m;
		m.m30 = vec.X;
		m.m31 = vec.Y;
		m.m32 = vec.Z;
		return m;
	}	
}