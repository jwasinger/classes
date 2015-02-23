#ifndef MATLIB_H
#define MATLIB_H

/*

Line and matrix header.

*/

void draw_line(double x0, double y0, double x1, double y1);
//int near_far_clip(double,double,double*,double*,double*,double*,double*,double*);
void osuInitialize();
void osuPushMatrix();
void osuPopMatrix();
void osuLoadIdentityMatrix();
void osuTranslate(double x, double y, double z);
void osuScale(double xs, double ys, double zs);
void osuRotate(double angle, double ax, double ay, double az);
void osuOrtho(double left, double right, double bottom, double top, double near, double far);
void osuPerspective(double fov, double near, double far);
void osuLookat(const Vector4 &from, const Vector4 &at, const Vector4 &up);
/*
void osuBegin(OSUDrawable);
void osuEnd();
*/
void osuVertex3f(double x, double y, double z);

#endif


