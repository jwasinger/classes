#ifndef MATLIB_H
#define MATLIB_H

#include <stdio.h>
#include <math.h>
#include <string>

#include "Matrix.h"
#include "MatrixStack.h"
/*

Line and matrix header.

*/

//int near_far_clip(double,double,double*,double*,double*,double*,double*,double*);
void osuPushMatrix();
void osuPopMatrix();
void osuLoadIdentityMatrix();
void osuTranslate(double x, double y, double z);
void osuScale(double xs, double ys, double zs);
void osuRotate(double angle, double ax, double ay, double az);
void osuOrtho(double left, double right, double bottom, double top, double near, double far);
void osuPerspective(double fov, double near, double far);
void osuLookat(const Vector4 &from, const Vector4 &at, const Vector4 &up);

extern MatrixStack mat_stack;

#endif


