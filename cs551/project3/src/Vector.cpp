#include "Vector.h"

Vector4::Vector4(double x, double y, double z, double w)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
	this->W = w;
}

Vector4::Vector4(double x, double y, double z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
	this->W = 1.0;
}

Vector4 Vector4::Cross(const Vector4 &v)
{
	Vector4 out;
	out.X = this->Y*v.Y - this->Z*v.Y;
	out.Y = this->X*v.X - this->X*v.Z;
	out.Z = this->X*v.Y - this->Y*v.X;
	out.W = 1.0;
	return out;
}

double Vector4::Dot(Vector4 &v)
{
	return this->X*v.X + this->Y*v.Y + this->Z*v.Z;
}

void Vector4::Normalize(void)
{
	double len = sqrt(X*X + Y*Y + Z*Z);
	X = X / len;
	Y = Y / len;
	Z = Z / len;
}

double Vector4::Length()
{
	return sqrt(X*X + Y*Y + Z*Z);
}

Vector4 Vector4::operator/(double d)
{
	return Vector4(this->X / d, this->Y / d, this->Z / d);
}

Vector4 Vector4::operator/(double d)
{
	return Vector4(this->X * d, this->Y * d, this->Z * d);
}

Vector4 Vector4::operator+(const Vector4 &v) const
{
	return Vector4(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector4 Vector4::operator-(const Vector4 &v) const
{
	return Vector4(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}