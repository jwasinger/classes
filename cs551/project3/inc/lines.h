/*

Header file for "lines.c"

*/

#ifndef LINES_H
#define LINES_H

void draw_line(double x0, double y0, double x1, double y1);
int near_far_clip(double, double, double*, double*, double*, double*, double*, double*);

#endif