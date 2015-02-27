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
#include <vector>
#include "matlib.h"

// osuImage Data Type Definition            

typedef struct osuVertex {
	float x, y, z;
	float r, g, b;
} osuVertex;

struct Material
{
	float ambient_i;
	Vector4 diffuse; // w-component unused
	Vector4 specular; // w-component used for Phong coefficient
};

extern Material NULLMaterial;

struct PLight
{
	Vector4 pos_i;
};

struct DLight
{
	Vector4 dir_i;
};

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
void osuFlush(void);
void osuWritePixel ( int x, int y, int r, int g, int b );
void osuSetWriteMode ( int /* OSUWriteMode */ mode );
void osuWaitOnEscape ();
void osuGetFramebufferSize ( int *w, int *h );

void osuEnd(void);

// Declarations of polygon drawing routines

enum OSUDrawable { OSU_NONE, OSU_TRIANGLE ,OSU_LINES, OSU_POLYGON };
enum OSUShadeModel { OSU_FLAT, OSU_SMOOTH };
void osuBegin ( int /* OSUDrawable */ );
void osuEnd (void);

void osuInitialize(void);

//void osuClear(float r, float g, float b);
void osuNormal3f(double x, double y, double z);
void osuEnable(int depthTestBit);
void osuShadeModel(int model);
void osuPointLight(PLight p);
void osuDirectionalLight(DLight d);
void osuAmbientLight(float i);
void osuVertex2f(double x, double y);
void osuVertex3f(double x, double y, double z);
void osuDiffuse(Vector4 diffuse_color);
void osuSpecular(Vector4 specular_color);
void osuFaceNormal3f(double x, double y, double z);

/*static Matrix CreateRotationAxisAngle(const Vector4 &axis, double angle);
static Matrix CreateTranslation(const Vector4 &vec);
static Matrix CreateScale(const Vector4 &vec);
static Matrix CreateOrthographicLH(double left, double right, double bottom, double top, double near, double far);

static Matrix CreatePerspectiveFOV(double fov, double aspect, double near, double far);

static Matrix CreateLookAt(Vector4 target, Vector4 eye, Vector4 up);*/

Vector4 toNDC(Vector4 world);

float osuGetZ(int x, int y);
void osuSetZ(int x, int y, float val);

void osuClearZ(void);
void init_z_buffer(int w, int h);
bool z_test(int x, int y);

extern int z_buffer_w, z_buffer_h;

void poly_clip(
	osuVertex *verts,
	int count,
	osuVertex *out_verts,
	int *out_count,
	float a,
	float b,
	float c,
	float d
	);

struct OSUDrawState
{
  std::vector<DLight> d_lights;
  std::vector<PLight> p_lights;
  int shade_model;
  double ambient_intensity;
  Material mat;
  bool z_test_enabled;
  Vector4 diffuse_color;
  Vector4 specular_color;
  
  int num_verts, num_normals;
  Vector4 normals[3];
  Vector4 vertices[3];
  Vector4 face_normal;
};
#endif  //end GFXSUPPORT

