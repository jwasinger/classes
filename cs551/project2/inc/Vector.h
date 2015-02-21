#ifndef VECTOR_H
#define VECTOR_H

#include <math.h>
#include <SimpleMath.h>

using namespace DirectX;

namespace A2Math
{
    class Vector4
    {
    public: 
		double X, Y, Z, W;

		Vector4(double x, double y, double z, double w);
		Vector4() { X = 0; Y = 0; Z = 0; W = 0; };
		double Dot(Vector4 &v);
        Vector4 Cross(const Vector4 &v);
		void Normalize(void);
		
		operator SimpleMath::Vector3();
		Vector4 operator+(const Vector4 &v) const;
		Vector4 operator/(double d);

        /*double X() const { return v_x; }
        double Y() const { return v_y; }
        double Z() const { return v_z; } 

		void X(double val) { v_x = val; }
		void Y(double val) { v_y = val; }
		void Z(double val) { v_z = val; }*/
	};

	/*class Vector4
	{
	public:
		double X, Y, Z, W;

		Vector4(double x, double y, double z, double w);
		Vector4();
		double Dot(const Vector4 &v) const;
		void Normalize(void);

		//perform cross product on first 3 components.  ignore w
		Vector4 Cross(const Vector4 &v) const;

		Vector4 operator+ (const Vector4 &v) const;
		Vector4 operator- (const Vector4 &v) const;
		Vector4 operator* (double d) const;
	};*/
}

#endif