/**
* OSU Graphics support functions
* Header File													
* 
* Author:	Turk, Metoyer
* Date:		1/9/07
*/

#ifndef OSUGFXSUPPORT
#define OSUGFXSUPPORT

#include <stdio.h>
#include "Vector.h"

// osuImage Data Type Definition            

typedef struct {
   // an image structure holds the image size 
   // and a dynamic array of pixels 
   int w, h;
   int *values;
} osuImage;

void osuImageInit ( osuImage *I );
void osuImageDestroy ( osuImage *I );
void osuImageGetSize ( osuImage *I, int *w, int *h );
void osuImageSetSize ( osuImage *I, int w, int h );
void osuImageWritePixel ( osuImage *I, int w, int h, int r, int g, int b );
void osuImageReadPixel ( osuImage *I, int w, int h, int *r, int *g, int *b );


// A Single Window Interface


enum OSUWriteMode { OSU_OVERWRITE, OSU_REPLACE, OSU_XOR };
enum OSUColor { OSU_RED, OSU_GREEN, OSU_BLUE };
enum OSUEnable { OSU_DEPTH_TEST };

void osuBeginGraphics ( int w, int h );
void osuEndGraphics ();
void osuClear ( int r, int g, int b );
void osuFlush();
void osuWritePixel ( int x, int y, int r, int g, int b );
void osuSetWriteMode ( int /* OSUWriteMode */ mode );
void osuWaitOnEscape ();
void osuGetFramebufferSize ( int *w, int *h );


// Declarations of polygon drawing routines

enum OSUDrawable { OSU_NONE, OSU_TRIANGLE ,OSU_LINES, OSU_POLYGON };

void osuBegin ( int /* OSUDrawable */ );
void osuEnd ();

//void osuClear(float r, float g, float b);
void osuNormal3f(double x, double y, double z);
void osuEnable(int depthTestBit);
void osuClearZ();
void osuShadeModel(int model);
void osuPointLight(double pos[3], double i);
void osuDirectionalLight(Vector4 &v, float i);
void osuAmbientLight(float i);
void osuVertex2f(double x, double y);
void osuDiffuse(unsigned short r, unsigned short g, unsigned short  b);
void osuSpecular(unsigned short r, unsigned short g, unsigned short b, float intensity);

#endif  //end GFXSUPPORT

