/*

Dummy routines for matrix transformations.

These are for you to write!

*/


#include <stdio.h>
#include <math.h>
#include <fstream>
#include <Windows.h>
#include <ostream>
#include <sstream>
#include <string>

#include "osuGraphics.h"
#include "lines.h"

#include "Matrix.h"
#include "MatrixStack.h"

//-------------------------------------------------

using namespace A2Math;


static int num_verts = 0;
static A2Math::Vector3 verts[2];

static MatrixStack mat_stack;
static Matrix cur_mat;
static const double pi = 3.14159;

void osuOrtho(double left, double right, double bottom, double top, double nearp,double farp)
{ 
	Matrix m = Matrix::CreateOrthographicLH(left, right, bottom, top, nearp, farp);
	//mat_stack.push_mat(m);
	mat_stack.set_proj(m);
	mat_stack.proj_type = PROJ_ORTHO;
}

void osuPerspective(double fovy, double nearp, double farp) 
{  	
	Matrix m = Matrix::CreatePerspectiveFOV(fovy * (pi/180.0), 1.0, nearp, farp);
	mat_stack.set_proj(m);
	mat_stack.proj_type = PROJ_PERSPECTIVE;
}

void osuBegin(OSUDrawable mode)
{
}

void osuEnd()
{
}


void osuColor3f(double red, double green, double blue)
{
}

void osuVertex2f(double x, double y)
{
}



void osuVertex3f(double x, double y, double z)
{
    int width, height;
	osuGetFramebufferSize(&width, &height);
	
	//A2Math::Matrix proj = A2Math::Matrix::CreateOrthographicLH(0.0, width, 0.0, height, 0.1, 1.0);
	//A2Math::Matrix transform = A2Math::Matrix::CreateTranslation(A2Math::MyVector3(0.1, 0.1, 0.0));
	//A2Math::Matrix rot = A2Math::Matrix::CreateRotationAxisAngle(A2Math::MyVector3(0, 0, 1), pi);
	//A2Math::Matrix proj = A2Math::Matrix::CreatePerspectiveFOV(pi / 2.0, 800.0 / 600.0, 1.0, 100.0);
	
	//mat_stack.set_proj(proj);
	//mat_stack.push_mat(transform);
	//mat_stack.push_mat(rot);

	if (num_verts == 0)
	{
		num_verts++;
		verts[0] = A2Math::Vector4(x, y, z);
	}
	else
	{
		A2Math::Matrix t = mat_stack.calc_top();
		A2Math::Matrix p = mat_stack.get_proj();

		verts[1] = A2Math::Vector4(x, y, z);
		num_verts = 0;

		//transform the vertices 
		//draw line using transformed vertices

		A2Math::Vector3 t_v1, t_v2, t1_v1, t1_v2;
		
		if (mat_stack.proj_type == PROJ_PERSPECTIVE)
		{
			double w;
			//perform transformation to clip space and then divide by w component
			//t.MulWithW(verts[0], t_v1, w);
			t_v1 = t * verts[0];

			p.MulWithW(t_v1, t1_v1, w);
			t1_v1 = t1_v1 / w;
		
			p.MulWithW(t_v2, t1_v2, w);
			t1_v2 = t1_v2 / w;
		}
		else
		{
			t_v1 = t * verts[0];
			t_v2 = t * verts[1];
		}

		//v1.Z(0.1);
		//v2.Z(0.1);

		//scale each vector component by frame buffer size
		/*transform the vector from the standard canonical viewing space:
			[(-1, 1), (-1, 1), (-1, 1)]
		to the space of the frame buffer:
			[(0,width), (0, height)]
		
		*/

		t_v1.X = t_v1.X + 1.0;
		t_v1.Y = t_v1.Y + 1.0;
		t_v2.X = t_v2.X + 1.0;
		t_v2.Y = t_v2.Y + 1.0;


		t_v1.X = t_v1.X * 0.5 * width;
		t_v1.Y = t_v1.Y * 0.5 *  height;
		t_v2.X = t_v2.X * 0.5 *  width;
		t_v2.X = t_v2.Y * 0.5 *  height;

		draw_line(t_v1.X, t_v1.Y, t_v2.X, t_v2.Y);
	}
}

void osuInitialize() 
{ 
}


void osuPushMatrix() 
{ 
	mat_stack.push_mat(cur_mat);
}

void osuPopMatrix() 
{ 
	mat_stack.pop_mat();
}

void osuLoadIdentityMatrix()
{
	Matrix m;
	mat_stack.push_mat(m);
}

void osuTranslate(double tx, double ty, double tz) 
{ 
	Matrix m = Matrix::CreateTranslation(A2Math::Vector3(tx, ty, tz));
	mat_stack.push_mat(m);
}

void osuScale(double sx, double sy, double sz) 
{ 
	Matrix m = Matrix::CreateScale(A2Math::Vector3(sx, sy, sz));
	mat_stack.push_mat(m);
}


void osuRotate(double angle, double ax, double ay, double az) 
{ 
	Matrix m = Matrix::CreateRotationAxisAngle(A2Math::Vector3(ax, ay, az), angle);
	
}

void osuLookat(double from[3], double at[3], double up[3])
{
}

