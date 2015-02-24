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

static struct DrawState
{
  std::vector<DLight> d_lights;
  std::vector<PLight> p_lights;
  int shade_model;
  int ambient_intensity;
  Material mat;
  bool z_test_enabled;
  Vector4 diffuse_color;
  Vector4 specular_color;
  
  int num_verts, num_normals;
  Vector4 normals[3];
  Vector4 verices[3];
};

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

void osuRasterizeTriangle(void)
{
  
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
