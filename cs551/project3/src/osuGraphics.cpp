/**
*	CS551 Graphics Support Code 
*	Implementation File 
*	Author:		Turk, Metoyer
*	Date:		1/10/07
*/

#include <windows.h>		// Header File For Windows
#include <gl\gl.h>			// Header File For The OpenGL32 Library
#include <gl\glu.h>			// Header File For The GLu32 Library		
#include <GL\freeglut.h>		//glut library
#include <stdio.h>
#include <malloc.h>
#include "osuGraphics.h"
#include "objLoader.h"
#include "matlib.h"

typedef unsigned char Byte;

void osuInitialize(void)
{
	//create the matrix stack and populate it with default values
}

// osuImage Data Type Implementation                                
long osuArray3DTo1D ( int chan, int w, int sizeW, int h, int sizeH )
{
   // convert 3D array coordinates to a single lookup value 
   return ( chan * sizeW * sizeH + w * sizeH + h );
}

/*-----------------------------------*/

long osuImageValueCount ( osuImage *I )
{
   return ( I->w * I->h * 3 );
}

/*-----------------------------------*/

void osuImageInit ( osuImage *I )
{
   I->w = I->h = 0;
   I->values = (int *) 0;
}

/*-----------------------------------*/

void osuImageDestroy ( osuImage *I )
{
   free( I->values );
}

/*-----------------------------------*/

void osuImageGetSize ( osuImage *I, int *w, int *h )
{
   *w = I->w;
   *h = I->h;
}

/*-----------------------------------*/

void osuImageSetSize ( osuImage *I, int w, int h )
{
   long count;

   free( I->values );

   I->w = w;
   I->h = h;
   count = osuImageValueCount( I );
   I->values = (int *) calloc( (unsigned int) count, sizeof( int ) );
   if ( ! I->values ) {
      fprintf( stderr, "osuImage not allocated\n" );
      exit( 2 );
   }
}

/*-----------------------------------*/


static OSUDrawState draw_state;
static float *z_data;
int z_buffer_w = 360;
int z_buffer_h = 360;

Material NULLMaterial =
{
	0.0,
	Vector4(0.0, 0.0, 0.0, 0.0),
	Vector4(0.0, 0.0, 0.0, 0.0)
};

void osuBegin(int flag)
{

}

void osuImageReadPixel ( osuImage *I, int w, int h, int *r, int *g, int *b )
{
   long index = osuArray3DTo1D( OSU_RED, w, I->w, h, I->h );
   *r = I->values[ index ];

   index = osuArray3DTo1D( OSU_GREEN, w, I->w, h, I->h );
   *g = I->values[ index ];

   index = osuArray3DTo1D( OSU_BLUE, w, I->w, h, I->h );
   *b = I->values[ index ];
}

/*-----------------------------------*/

void osuImageWritePixel ( osuImage *I, int w, int h, int r, int g, int b )
{
   long index;

   index = osuArray3DTo1D( OSU_RED, w, I->w, h, I->h );
   I->values[ index ] = r;

   index = osuArray3DTo1D( OSU_GREEN, w, I->w, h, I->h );
   I->values[ index ] = g;

   index = osuArray3DTo1D( OSU_BLUE, w, I->w, h, I->h );
   I->values[ index ] = b;
}


// Single Window Interface 

static int /* OSUWriteMode */ OSUCurrentMode;
static osuImage OSUCurrentImage;

void osuBeginGraphics ( int w, int h )
{
	int argc = 1;
	char *argv = "Foo" ;
	glutInit(&argc,&argv);

	// set up a window with RGB color and one buffer 
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(w,h);
	glutInitWindowPosition(0,0);
	glutCreateWindow("CS551 OSUGL");
    gluOrtho2D( -0.5, w - 0.5, -0.5, h - 0.5 );

   //	set the single window state 
   OSUCurrentMode = OSU_OVERWRITE;

   //	build a frame buffer for reading and writing 
   osuImageInit( &OSUCurrentImage );
   osuImageSetSize( &OSUCurrentImage, w, h );
   osuClear( 0, 0, 0 );

   //set the draw state to a default value
   draw_state.ambient_intensity = 0.3;
   draw_state.diffuse_color = Vector4(1.0, 0.0, 0.0);
   draw_state.mat = Material();
   draw_state.mat.ambient_i = 0.5;
   draw_state.mat.diffuse = Vector4(1.0, 0.0, 0.0);
   draw_state.mat.specular = Vector4(0.0, 1.0, 0.0);
   draw_state.num_normals = 0;
   draw_state.num_verts = 0;
}

/*-----------------------------------*/

void osuGetFramebufferSize ( int *w, int *h )
{
   *w = OSUCurrentImage.w;
   *h = OSUCurrentImage.h;
}

/*-----------------------------------*/

void osuEndGraphics ( void )
{
   osuImageDestroy( &OSUCurrentImage );
}

/*-----------------------------------*/

void osuClear ( int r, int g, int b )
{
   int i, j;


   glClearColor( (float) r, (float) g, (float) b, 0.0 );
   glClear( GL_COLOR_BUFFER_BIT );

   // clear image 
   for ( i = 0; i < OSUCurrentImage.w; i++ )
      for ( j = 0; j < OSUCurrentImage.h; j++ )
         osuImageWritePixel( &OSUCurrentImage, i, j, r, g, b );
}

/*-----------------------------------*/

void osuFlush ()
{
   glFlush ();
}

/*-----------------------------------*/

void osuSetWriteMode ( int /* OSUWriteMode */ mode )
{
   OSUCurrentMode = mode;
}

/*-----------------------------------*/

int osuXOR ( int a, int b )
{
   return ( a ^ b);
}


/*-----------------------------------*/

void osuWritePixel ( int x, int y, int r, int g, int b )
{
   int or, og, ob;

   if ( x < 0 || x >= OSUCurrentImage.w || y < 0 || y >= OSUCurrentImage.h )
   {
     fprintf( stderr, "Attempted to write a pixel outside the image " );
     fprintf( stderr, "bounds: x = %d, y = %d\n", x, y );
     return;
   }

   if ( OSUCurrentMode == OSU_XOR )
   {
      osuImageReadPixel( &OSUCurrentImage, x, y, &or, &og, &ob );
      r = osuXOR ( r, or );
      g = osuXOR ( g, og );
      b = osuXOR ( b, ob );
   }


   glColor3f( r / 255.0, g / 255.0, b / 255.0 );
   glBegin( GL_POINTS );
     glVertex2i( x, y );
   glEnd();


   osuImageWritePixel( &OSUCurrentImage, x, y, r, g, b );
}

/*-----------------------------------*/

void osuRedraw ( void )
{
   int x, y;
   int r, g, b;

   // Reapply fake 2D screen - problems occur otherwise 
   // Nothing is redrawn if this is used

   glClear(GL_COLOR_BUFFER_BIT);
   glBegin( GL_POINTS );

      for ( x = 0; x < OSUCurrentImage.w; x++ ) {
         for ( y = 0; y < OSUCurrentImage.h; y++ ) {
            osuImageReadPixel( &OSUCurrentImage, x, y, &r, &g, &b );
            glColor3f( r / 255.0, g / 255.0, b / 255.0 );
            glVertex2i( x, y );
		}
     }

   glEnd();
   glFlush();
}


void keyPress(char key, int x, int y)
{
	switch (key)
	{
		case 'q':
			exit(0);
		default:
			printf("Press 'q' key to quit \n");

	}
}

/*-----------------------------------*/

void osuWaitOnEscape ()
{   
	glutKeyboardFunc((void (*) (unsigned char, int, int) )keyPress);
	glutDisplayFunc(osuRedraw);
	glutMainLoop();
}

/*-----------------------------------*/

void osuFaceNormal3f(double x, double y, double z)
{
	draw_state.face_normal = Vector4(x, y, z);
}

void osuNormal3f(double x, double y, double z)
{
  if(draw_state.num_normals >= 3)
  {
    OutputDebugString(L"Attempting to add more than 3 normals before rendering a triangle...\n");
    return;
  }

  Vector4 normal = Vector4(x,y,z);
  draw_state.normals[draw_state.num_normals] = normal;
  draw_state.num_normals++;
}

//find the area of a 3D triangle assuming that v1,2,3 are defined in clockwise order
double tri_area2D(Vector4 v0, Vector4 v1, Vector4 v2)
{
	return abs((v0.X*(v1.Y - v2.Y) + v1.X*(v2.Y - v0.Y) + v2.X*(v0.Y - v1.Y)) / 2.0);
}

void edge_dist_2D(Vector4 pos, Vector4 a, Vector4 b, Vector4 c, double &u, double &v, double &w)
{
	Vector4 v0 = Vector4(pos.X - a.X, pos.Y - a.Y, 0.0);
	Vector4 v1 = Vector4(b.X - a.X, b.Y - a.Y, 0.0);
	Vector4 v2 = Vector4(c.X - a.X, c.Y - a.Y, 0.0);

	double d20 = v2.Dot(v0);
	double d12 = v1.Dot(v2);
	double d22 = v2.Dot(v2);
	double d10 = v1.Dot(v0);
	double d11 = v1.Dot(v1);
	double d21 = v2.Dot(v1);

	double denom = d22*d11 - d21*d12;

	v = (d10*d22 - d20*d21) / denom;
	w = (d20*d11 - d10*d12) / denom;
	u = 1.0 - (v + w);

	if (u == 0) //distance from top: 12
	{
		int a = 0;
	}
	if (w == 0) //distance from right 01
	{
		int b = 0;
	}
	if (v == 0) //distance from diagonal 20
	{
		int c = 0;
	}
}

osuVertex Vector4_to_osuVertex(Vector4 v)
{
	osuVertex out;
	out.x = v.X;
	out.y = v.Y;
	out.z = v.Z;

	return out;
}

void vector_append_array(std::vector<Vector4> vec, osuVertex* verts, int num_verts)
{
	for (int i = 0; i < num_verts; i++)
	{
		Vector4 vert = Vector4(verts[i].x, verts[i].y, verts[i].z);
		vec.push_back(vert);
	}
}

//TODO: this function still needs to account for d component of plane equation!!!!1
//Also doesn't account for the normals of clipped triangles.  What are they?
void clip_triangle(Vector4 tri[3], std::vector<Vector4> out_verts)
{
	out_verts = std::vector<Vector4>();

	osuVertex *in_verts = (osuVertex *)malloc(sizeof(osuVertex) * 3);
	osuVertex *out_osu_verts = (osuVertex *)malloc(sizeof(osuVertex) * 6);
	int num_verts;

	in_verts[0] = Vector4_to_osuVertex(tri[0]);
	in_verts[1] = Vector4_to_osuVertex(tri[1]);
	in_verts[2] = Vector4_to_osuVertex(tri[2]);


	Vector4 top_v, bottom_v, left_v, right_v, near_v, far_v;
	top_v = Vector4(0.0, -1.0, 0.0);
	bottom_v = Vector4(0.0, 1.0, 0.0);
	left_v = Vector4(1.0, 0.0, 0.0);
	right_v = Vector4(-1.0, 0.0, 0.0);
	near_v = Vector4(0.0, 0.0, -1.0);
	far_v = Vector4(0.0, 0.0, 1.0);

	//clip against the left plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, left_v.X, left_v.Y, left_v.Z, left_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;
	
	//clip against the right plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, right_v.X, right_v.Y, right_v.Z, right_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;

	//clip against the top plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, top_v.X, top_v.Y, top_v.Z, top_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;

	//clip against the bottom plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, bottom_v.X, bottom_v.Y, bottom_v.Z, bottom_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;

	//clip against the near plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, near_v.X, near_v.Y, near_v.Z, near_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;

	//clip against the far plane
	poly_clip(in_verts, 3, out_osu_verts, &num_verts, far_v.X, far_v.Y, far_v.Z, far_v.W);
	vector_append_array(out_verts, out_osu_verts, num_verts);

	memset(out_osu_verts, 0, sizeof(osuVertex) * 6);
	num_verts = 0;
}

Vector4 toNDC(Vector4 world)
{
	Vector4 result;
	Matrix t = mat_stack.calc_val();
	Matrix p = mat_stack.get_proj();
	
	result = t * world;
	result = p * result;
	result = result / result.W;

	return result;
}

void draw_tri(void)
{
	std::vector<Vector4> all_vertices;
	clip_triangle(draw_state.vertices, all_vertices); 

	for (int i = 0; i < all_vertices.size() / 3; i++)
	{
		
	}
}

void bounding_box(Vector4 v0,
				  Vector4 v1, 
				  Vector4 v2, 
				  double *origin_x, 
				  double *origin_y, 
				  double *width, 
				  double *height)
{
	//find the left-top-most point

	if (v0.X <= v1.X)
		*origin_x = v0.X;
	if (v2.X <= v0.X)
		*origin_x = v2.X;
	if (v0.Y >= v1.Y)
		*origin_y = v1.Y;
	if (v2.Y <= v0.Y)
		*origin_y = v2.Y;
	
	*width = abs(v0.X - *origin_x);
	
}

void calc_diffuse(Vector4 normal, Vector4 &out_color)
{
	double pi = 3.14159;

	//only use one directional light
	Vector4 light_dir = draw_state.d_lights[0].dir_i;
	double angle = asin(normal.Dot(light_dir) / (light_dir.Length() * normal.Length()));
	out_color = 
		draw_state.diffuse_color * 
		(((pi / 2.0) - angle) / (pi / 2.0)) * 
		draw_state.ambient_intensity;
}

void calc_specular(Vector4 normal, Vector4 &out_color)
{
	
}

void osuSetZ(int x, int y, float val)
{
	z_data[y * z_buffer_w + x] = val;
}

float osuGetZ(int x, int y)
{
	return z_data[y * z_buffer_w + x];
}

void osuEnd(void)
{

}

//consider the triangle to be in 2D coordinates
bool z_test(int x, int y, Vector4 t_verts[3])
{
	Vector4 pos = Vector4(x, y, 0.0);
	double area_total = tri_area2D(t_verts[0], t_verts[1], t_verts[2]);

	double dz = 
		t_verts[2].Z * (tri_area2D(pos, t_verts[0], t_verts[1]) / area_total) +
		t_verts[1].Z * (tri_area2D(pos, t_verts[0], t_verts[2]) / area_total) +
		t_verts[0].Z * (tri_area2D(pos, t_verts[1], t_verts[2]) / area_total);
	
	float existing_val = osuGetZ(x, y);

	if (existing_val > abs(dz))
	{
		osuSetZ(x, y, abs(dz));
		return true;
	}

	return false;
}

bool in_range_with_error(double val, double low, double high, double error)
{
	if (val >= low - error) //&& val <= high + error)
		return true;
	return false;
}

bool rasterize_pixel(int x, int y, Vector4 &out_color)
{
	//see if the pixel falls inside the triangle
	double u, v, w;
	Vector4 pos = Vector4(x, y, 0.0);
	Vector4 diffuse_color;
	Vector4 specular_color;

	Vector4 t_verts[3];
	memcpy(draw_state.vertices, t_verts, sizeof(Vector4) * 3);

	t_verts[0] = toNDC(t_verts[0]);
	t_verts[1] = toNDC(t_verts[1]);
	t_verts[2] = toNDC(t_verts[2]);

	edge_dist_2D(pos, t_verts[0], t_verts[1], t_verts[2], u, v, w);

	bool u_cond, v_cond, w_cond;
	u_cond = in_range_with_error(u, 0.0, 1.0, 0);
	v_cond = in_range_with_error(v, 0.0, 1.0, 0);
	w_cond = in_range_with_error(w, 0.0, 1.0, 0);

	if (!u_cond || !v_cond || !w_cond)
		return false;

	//construct a bounding box around the triangle and rasterize the pixels contained by the box

	//see if the pixel passes the z-test
	if (!z_test(x, y, t_verts))
	{
		return false;
	}

	//find the position of the pixel on the triangle in 3D space
	Vector4 pos_3D;
	double a0, a1, a2;
	a0 = tri_area2D(draw_state.vertices[2], pos_3D, draw_state.vertices[1]);
	a1 = tri_area2D(pos_3D, draw_state.vertices[2], draw_state.vertices[0]);
	a2 = tri_area2D(pos_3D, draw_state.vertices[0], draw_state.vertices[1]);

	double total_area = tri_area2D(draw_state.vertices[0], draw_state.vertices[1], draw_state.vertices[2]);

	//interpolate normals based on a*/total_area
	if (draw_state.shade_model == OSU_FLAT)
	{
		calc_diffuse(draw_state.face_normal, diffuse_color);
	}
	else
	{
	}

	Vector4 diffuse = diffuse_color * 0.5;
	out_color = Vector4(1.0, 1.0, 1.0) * 0.3 + diffuse;
	return true;
}

void rasterize_tri3D(Vector4 v0, Vector4 v1, Vector4 v2)
{
	//construct the bounding box
	//TODO: complete the algorithm
	Vector4 color;
	int r, g, b;

	for (int x = 0; x < 360; x++)
	{
		for (int y = 0; y < 360; y++)
		{
			if (rasterize_pixel(x, y, color))
			{
				r = color.X * 255;
				g = color.Y * 255;
				b = color.Z * 255;
				osuWritePixel(x, y, r, g, b);
			}
		}
	}

	
}

void osuVertex3f(double x, double y, double z)
{
  if(draw_state.num_verts == 2)
  {
    Vector4 vertex = Vector4(x, y, z);
	draw_state.vertices[draw_state.num_verts - 1] = vertex;
	draw_state.num_verts++;

	rasterize_tri3D(draw_state.vertices[0], draw_state.vertices[1], draw_state.vertices[2]);

    draw_state.num_verts = 0;
    draw_state.num_normals = 0;
    return;
  }
  
  Vector4 vertex = Vector4(x,y,z);
  draw_state.vertices[draw_state.num_verts-1] = vertex;
  draw_state.num_verts++;
}

void osuEnable(int depthTestBit)
{
  draw_state.z_test_enabled = true;  
}


void init_z_buffer(int w, int h)
{
  z_data = (float *)malloc(sizeof(float) * z_buffer_w * z_buffer_h);
  ZeroMemory(z_data, sizeof(float) * w * h);
}

void osuClearZ()
{
  ZeroMemory(z_data, sizeof(float) * z_buffer_w * z_buffer_h);
}

void osuShadeModel(int model)
{
  draw_state.shade_model = model;
}

void osuPointLight(PLight p)
{
  draw_state.p_lights.push_back(p);
}

void osuDirectionalLight(DLight l)
{
  draw_state.d_lights.push_back(l);
}

void osuAmbientLight(float i)
{
  draw_state.ambient_intensity = i;
}

void osuVertex2f(double x, double y)
{}

void osuDiffuse(Vector4 diffuse_color)
{
  draw_state.diffuse_color = diffuse_color;
}

void osuSpecular(Vector4 specular_color)
{
  draw_state.specular_color = specular_color;
}
