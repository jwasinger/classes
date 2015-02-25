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

typedef unsigned char Byte;

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
}

/*-----------------------------------*/

void osuGetFramebufferSize ( int *w, int *h )
{
   *w = OSUCurrentImage.w;
   *h = OSUCurrentImage.h;
}

/*-----------------------------------*/

void osuEndGraphics ( int w, int h )
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


static DrawState draw_state;
static float *z_data;

void osuNormal3f(double x, double y, double z)
{
  if(num_normals >= 3)
  {
    OutputDebugString("Attempting to add more than 3 normals before rendering a triangle...\n");
    return;
  }

  Vector4 normal = Vector4(x,y,z);
  draw_state.normals[num_normals-1] = normal;
  num_normals++;
}

double tri_area3D(Vector2 v0, Vector2 v1, Vector2 v2)
{
	return abs((v0.x*(v1.y - v2.y) + v1.x*(v2.y - v0.y) + v2.x*(v0.y - v1.y)) / 2.0);
}

void barycentric3D(Vector2 pos, Vector2 a, Vector2 b, Vector2 c, double &u, double &v, double &w)
{
	Vector2 v0 = { pos.x - a.x, pos.y - a.y };
	Vector2 v1 = { b.x - a.x, b.y - a.y };
	Vector2 v2 = { c.x - a.x, c.y - a.y };

	double d20 = dot(v2, v0);
	double d12 = dot(v1, v2);
	double d22 = dot(v2, v2);
	double d10 = dot(v1, v0);
	double d11 = dot(v1, v1);
	double d21 = dot(v2, v1);

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

void clip_and_rasterize(ObjTriangle &tri, ObjModel &model)
{
  //perform clipping
  //rasterize 
}

void __rasterize_tri(ObjTriangle &tri, ObjModel &model)
{
	double u, v, w;
	barycentric(pos, v0.Position, v1.Position, v2.Position, u, v, w);

	bool u_cond, v_cond, w_cond;
	u_cond = in_range_with_error(u, 0.0, 1.0, 0);
	v_cond = in_range_with_error(v, 0.0, 1.0, 0);
	w_cond = in_range_with_error(w, 0.0, 1.0, 0);
	
	if (u_cond && v_cond && w_cond)
	{

		double a0, a1, a2;
		a0 = tri_area(v2.Position, pos, v1.Position);
		a1 = tri_area(pos, v2.Position, v0.Position);
		a2 = tri_area(pos, v0.Position, v1.Position);

		double total_area = tri_area(v0.Position, v1.Position, v2.Position);
    
    //interpolate normals based on a*/total_area
    if(draw_state.shade_model == OSU_FLAT)
    {
      
    }
    else
    {
    
    }

    //construct a bounding box around the triangle and rasterize the pixels contained by the box
		return true;
	}
	return false;
}


void osuVertex3f(double x, double y, double z)
{
  if(num_verts == 3)
  {
    //transform the vertex to Window space
    //rasterize the triangle using the z-buffer test and lighting
    osuRasterizeTriangle();
    num_verts = 0;
    num_normals = 0;
    return;
  }
  
  Vector4 vertex = Vector4(x,y,z);
  draw_state.vertices[num_verts-1] = vertex;
  draw_state.num_verts++;
}

void osuEnable(int depthTestBit)
{
  draw_state.z_test_enabled = true;  
}


void init_z_buffer(int w, int h)
{
  z_data = malloc(sizeof(float) * w * h);
  ZeroMemory(z_data, sizeof(float) * w * h);
}

void osuClearZ()
{
  ZeroMemory(z_data, sizeof(float) * w * h);
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
