#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>

class Vector4
{
public: 
	double X, Y, Z, W;

	Vector4(double x, double y, double z, double w);
	Vector4(double x, double y, double z);
	Vector4() { X = 0; Y = 0; Z = 0; W = 0; };
	double Dot(Vector4 &v);
    Vector4 Cross(const Vector4 &v);
	void Normalize(void);
	double Length(void);

	Vector4 operator*(double d) const;
	Vector4 operator+(const Vector4 &v) const;	
	Vector4 operator-(const Vector4 &v) const;
	Vector4 operator/(double d);
};

#endif