#include "matlib.h"

static const double pi = 3.14159;
MatrixStack mat_stack = MatrixStack();

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

/*
void osuBegin(OSUDrawable mode)
{
}

void osuEnd()
{
}
*/




/*void osuVertex3f(double x, double y, double z)
{
    int width, height;
	osuGetFramebufferSize(&width, &height);

	if (num_verts == 0)
	{
		num_verts++;
		verts[0] = Vector4(x, y, z, 1.0);
	}
	else
	{
		Matrix t = mat_stack.calc_val();
		Matrix p = mat_stack.get_proj();

		verts[1] = Vector4(x, y, z, 1.0);
		num_verts = 0;

		//transform the vertices 
		//draw line using transformed vertices

		Vector4 t_v1, t_v2;
		
		if (mat_stack.proj_type == PROJ_PERSPECTIVE)
		{
			double w;
			//perform transformation to clip space and then divide by w component
			//t.MulWithW(verts[0], t_v1, w);
			t_v1 = t * verts[0];
			t_v1 = p * t_v1;

			t_v1 = t_v1 / t_v1.W;
		
			t_v2 = t * verts[1];
			t_v2 = p * t_v2;

			t_v2 = t_v2 / t_v2.W;
		}
		else
		{
			t_v1 = t * verts[0];
			t_v2 = t * verts[1];
			t_v1 = p * t_v1;
			t_v2 = p * t_v2;
		}

		t_v1.X = t_v1.X + 1.0;
		t_v1.Y = t_v1.Y + 1.0;
		t_v2.X = t_v2.X + 1.0;
		t_v2.Y = t_v2.Y + 1.0;
		

		t_v1.X = t_v1.X * 0.5 * width;
		t_v1.Y = t_v1.Y * 0.5 *  height;
		t_v2.X = t_v2.X * 0.5 *  width;
		t_v2.Y = t_v2.Y * 0.5 *  height;

		if (near_far_clip(-1.0, 1.0, &t_v1.X, &t_v1.Y, &t_v1.Z, &t_v2.X, &t_v2.Y, &t_v2.Z) == 1)
			draw_line(t_v1.X, t_v1.Y, t_v2.X, t_v2.Y);
	}
}*/


void osuPushMatrix() 
{ 
	//mat_stack.push_mat(cur_mat);
	//cur_mat = Matrix();
	Matrix m;
	mat_stack.push_mat(m);
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
	/*Matrix m = Matrix::CreateTranslation(A2Math::Vector4(tx, ty, tz, 1.0));
	cur_mat = cur_mat * m;*/
	Matrix &m = mat_stack.get_top();
	m = m * Matrix::CreateTranslation(Vector4(tx, ty, tz, 1.0));
}

void osuScale(double sx, double sy, double sz) 
{ 
	/*Matrix m = Matrix::CreateScale(A2Math::Vector4(sx, sy, sz, 1.0));
	cur_mat = cur_mat * m;*/

	Matrix &m = mat_stack.get_top();
	m = m * Matrix::CreateScale(Vector4(sx, sy, sz, 1.0));
}


void osuRotate(double angle, double ax, double ay, double az) 
{ 
	/*Matrix m = Matrix::CreateRotationAxisAngle(A2Math::Vector4(ax, ay, az, 1.0), angle);
	cur_mat = cur_mat * m;*/
	Matrix &m = mat_stack.get_top();
	angle = angle * (pi / 180);
	m = m * Matrix::CreateRotationAxisAngle(Vector4(ax, ay, az, 1.0), angle);
}

void osuLookat(double from[3], double at[3], double up[3])
{
	Matrix &m = mat_stack.get_top();
	m = m * Matrix::CreateLookAt(Vector4(at[0], at[1], at[2], 1.0),
		Vector4(from[0], from[1], from[2], 1.0),
		Vector4(up[0], up[1], up[2], 1.0)
		);
}

