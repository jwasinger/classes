/*

Test the polygon scan conversion routines.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "osuGraphics.h"

#include <vector>

void osuColor3f (double, double, double);
void osuVertex2f (double, double);
void osuBegin (int);
void osuEnd();
void simple_triangle();
void abutting_triangles();
void color_polygons();
void wedges(double,double,double,int);
void overlapTest();

struct Vector2
{
	double x, y;
};

struct Color
{
	double r, g, b;
};

struct Vertex
{
	Vector2 Position;
	Color Color;
};

struct AppState
{
	OSUDrawable mode;
	Color color;
};

std::vector<Vertex> verts;
AppState cur_state;

// BEGIN my functions
int unclamp(double val, int range)
{
	return (int)(val * range);
}

//clamp value from [0,255] to [0.0f, 1.0f]
double clamp(int val)
{
	return (double)val / 255.0;
}

double dot(Vector2 a, Vector2 b)
{
	return a.x*b.x + a.y*b.y;
}

//ask me to derive if necessary.  Source: 
void barycentric(Vector2 pos, Vector2 a, Vector2 b, Vector2 c, double &u, double &v, double &w)
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

bool equals_with_error(double val, double expected, double error)
{
	double low = expected - error;
	double high = expected + error;
	if (val > low && val < high)
	{
		return true;
	}
	
	return false;
}

//source: http://www.mathopenref.com/coordtrianglearea.html
double tri_area(Vector2 v0, Vector2 v1, Vector2 v2)
{
	return abs((v0.x*(v1.y - v2.y) + v1.x*(v2.y - v0.y) + v2.x*(v0.y - v1.y)) / 2.0);
}

Color rasterize_line(Vector2 pos, Vertex v0, Vertex v1)
{
	double x_max, y_max, x_min, y_min;
	if (v1.Position.x > v0.Position.x)
	{
		x_max = v1.Position.x;
		x_min = v0.Position.x;
	}
	else
	{
		x_max = v0.Position.x;
		x_min = v1.Position.x;
	}

	if (v1.Position.y > v0.Position.y)
	{
		y_max = v1.Position.y;
		y_min = v0.Position.y;
	}
	else
	{
		y_max = v0.Position.y;
		y_min = v1.Position.y;
	}

	double slope = (v1.Position.y - v0.Position.y) / (v1.Position.x - v0.Position.x);
	double y_int = v1.Position.y - slope*v1.Position.x;

	if (equals_with_error(pos.y, slope*pos.x + y_int, 0.001) && pos.y <= y_max && pos.y >= y_min && pos.x <= x_max && pos.x >= x_min)
	{
		return { 1.0, 0.0, 0.0 }; //TODO: Interpolate colors
	}

	return{ 0.0, 0.0, 0.0 };
}

bool in_range_with_error(double val, double low, double high, double error)
{
	if (val >= low - error) //&& val <= high + error)
		return true;
	return false;
}

bool rasterize_tri(Vector2 pos, Vertex v0, Vertex v1, Vertex v2, Color &output)
{
	double u, v, w;
	barycentric(pos, v0.Position, v1.Position, v2.Position, u, v, w);

	bool u_cond, v_cond, w_cond;
	u_cond = in_range_with_error(u, 0.0, 1.0, 0);
	v_cond = in_range_with_error(v, 0.0, 1.0, 0);
	w_cond = in_range_with_error(w, 0.0, 1.0, 0);
	
	if (u_cond && v_cond && w_cond)
	{
		//interpolate colors

		/*double sat0, sat1, sat2;
		double d0, d1, d2;
		d0 = sqrt(pow(pos.x - v0.Position.x, 2) + pow(pos.y - v0.Position.y, 2));
		d1 = sqrt(pow(pos.x - v1.Position.x, 2) + pow(pos.y - v1.Position.y, 2));
		d2 = sqrt(pow(pos.x - v2.Position.x, 2) + pow(pos.y - v2.Position.y, 2));
		*/
		//double total = d0 + d1 + d2;

		double a0, a1, a2;
		a0 = tri_area(v2.Position, pos, v1.Position);
		a1 = tri_area(pos, v2.Position, v0.Position);
		a2 = tri_area(pos, v0.Position, v1.Position);

		double total_area = tri_area(v0.Position, v1.Position, v2.Position);

		/*output = { (d0 / total) * v0.Color.r + (d1 / total) * v1.Color.r + (d2 / total) * v2.Color.r,
				   (d0 / total)* v0.Color.g + (d1 / total) * v1.Color.g + (d2 / total) * v2.Color.g,
				   (d0 / total)* v0.Color.b + (d1 / total) * v1.Color.b + (d2 / total) * v2.Color.b };
		*/
		output.r = v0.Color.r * (a0 / total_area) + v1.Color.r * (a1 / total_area) + v2.Color.r * (a2 / total_area);
		output.g = v0.Color.g * (a0 / total_area) + v1.Color.g * (a1 / total_area) + v2.Color.g * (a2 / total_area);
		output.b = v0.Color.b * (a0 / total_area) + v1.Color.b * (a1 / total_area) + v2.Color.b * (a2 / total_area);

		return true;
	}
	return false;
}

// END my functions
void osuBegin(OSUDrawable mode)
{
	cur_state.mode = mode;
	//cur_state.color = { 0.0f, 0.0f, 0.0 };

	verts = std::vector<Vertex>(); //empty the vector
}


void osuColor3f(double red, double green, double blue)
{
	cur_state.color = { red, green, blue };
}

void osuVertex2f(double x, double y)
{
	Vertex v = { { x, y }, cur_state.color };
	verts.push_back(v);
}


void osuEnd()
{
	int width, height;
	Vector2 pos;
	Color output;
	osuGetFramebufferSize(&width, &height);

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pos = { clamp(j), clamp(i) };
			if (cur_state.mode == OSUDrawable::OSU_TRIANGLE)
			{
				for (int k = 0; k < verts.size(); k += 3)
				{
					if(rasterize_tri(pos, verts[k], verts[k + 1], verts[k + 2], output))
						osuWritePixel(j, i, unclamp(output.r, 255), unclamp(output.g, 255), unclamp(output.b, 255));
					//osuWritePixel(pos.x, pos.y, 0.0, 1.0, 0.0);
				}
			}
			else if (cur_state.mode == OSUDrawable::OSU_LINES)
			{
				for (int k = 0; k < verts.size() - 1; k++)
				{
					output = rasterize_line(pos, verts[k], verts[k + 1]);
					osuWritePixel(j, i, unclamp(output.r, 255), unclamp(output.g, 255), unclamp(output.b, 255));
				}
			}
		}
	}
}

/******************************************************************************
Draw a polygon.
*****************************************************************************/
void main(int argc , char **argv)
{
	int num = atoi(argv[1]);

	if((num <0 ) || (num > 5))
	{
		fprintf(stderr, "Please call this program with a number from 1 to 5 \n");
		exit(-1);
	}

  osuBeginGraphics (500, 500);

  /* go to selected routine */

  //osuSetWriteMode(OSU_XOR);
  //osuSetWriteMode(OSU_OVERWRITE);
  /*osuBegin(OSUDrawable::OSU_TRIANGLE);
  cur_state.color = { 1.0, 0.0, 0.0 };
  
  osuColor3f(0.5, 0.5, 0.0);
  osuVertex2f(0.0, 0.0);
  
  osuColor3f(1.0, 0.0, 0.0);
  osuVertex2f(1.0, 0.0);
  
  osuColor3f(0.0, 0.0, 1.0);
  osuVertex2f(0.0, 1.0);
  
  osuColor3f(1.0, 0.0, 0.0);
  osuVertex2f(1.0, 0.0); //right

  osuColor3f(0.0, 1.0, 0.0);
  osuVertex2f(1.0, 1.0); //up-right
  
  osuColor3f(0.0, 0.0, 1.0);
  osuVertex2f(0.0, 1.0); //up

  osuEnd();*/

  switch (num) {
      case 1:
      simple_triangle();
      break;
    case 2:
		osuSetWriteMode(OSU_XOR);
      abutting_triangles();
      break;
    case 3:
		osuSetWriteMode(OSU_XOR);
      color_polygons();
      break;
	 case 4:
	   osuColor3f(1.0, 1.0,1.0);
	   osuSetWriteMode(OSU_XOR);
	   wedges(0.501, 0.497, 0.4, 13); 
	   break;
	 case 5:
		//overlap
	   osuSetWriteMode(OSU_XOR);
		overlapTest();
		break;
    default:
      fprintf (stderr, "Please use a number from 1 to 5.\n");
      exit (-1);
  }

  osuFlush();
  osuWaitOnEscape();
  osuEndGraphics();
}


/******************************************************************************
Draw a white triangle.
******************************************************************************/

void simple_triangle()
{
  osuBegin (OSU_TRIANGLE);
  osuColor3f (1.0, 1.0, 1.0);
  osuVertex2f (0.25, 0.25);
  osuVertex2f (0.25, 0.75);
  osuVertex2f (0.75, 0.5);
  osuEnd ();
}


/******************************************************************************
Draw several abutting rectangles.
******************************************************************************/

void abutting_triangles()
{
  double a = 0.1; 
  double b = 0.5;
  double c = 0.9;

  //#ifdef NOTDEF
  osuBegin (OSU_TRIANGLE);
  osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (a, a);
	osuVertex2f (b, b);
	osuVertex2f (a, c);
  osuEnd();
 
  osuBegin(OSU_TRIANGLE);
  osuColor3f(1.0, 0.0, 0.0);
	osuVertex2f(b,b);
	osuVertex2f(c,c);
	osuVertex2f (a,c);
  osuEnd();


  osuBegin(OSU_TRIANGLE);
  osuColor3f(0.0, 1.0, 0.0);
	osuVertex2f(b,b);
	osuVertex2f(c,c);
	osuVertex2f (c,a);
  osuEnd();


  osuBegin(OSU_TRIANGLE);
  osuColor3f(0.0, 0.0, 1.0);
	osuVertex2f(a,a);
	osuVertex2f(c,a);
	osuVertex2f (b,b);
  osuEnd();

}



/******************************************************************************
Draw some triangles that use color interpolation.
******************************************************************************/

void color_polygons()
{
  double x0 = 0.6;
  double y0 = 0.6;
  double x1 = 0.9;
  double y1 = 0.9;
  double x2,y2;

  /* colorful triangle */

  osuBegin (OSU_TRIANGLE);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (0.1, 0.1);
	osuColor3f (0.0, 1.0, 0.0);
	osuVertex2f (0.1, 0.5);
	osuColor3f (0.0, 0.0, 1.0);
	osuVertex2f (0.5, 0.3);
  osuEnd ();

  /* colors for square */
  osuBegin (OSU_TRIANGLE);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x0, y0);
	osuColor3f (0.0, 1.0, 0.0);
	osuVertex2f (x0, y1);
	osuColor3f (0.0, 0.0, 1.0);
	osuVertex2f (x1, y1);
  osuEnd();

  osuBegin(OSU_TRIANGLE);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x0, y0);
	osuColor3f(0.0, 0.0, 1.0);
	osuVertex2f(x1,y1);
	osuColor3f(1.0, 1.0, 1.0);
	osuVertex2f(x1, y0);
  osuEnd ();


  x0 = 0.55;
  y0 = 0.15;
  x1 = 0.7;
  y1 = 0.3;
  x2 = 0.85;
  y2 = 0.45;

  osuBegin (OSU_TRIANGLE);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x0, y1);
	osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x1, y0);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x2, y1);
  osuEnd();

  osuBegin(OSU_TRIANGLE);
	osuColor3f(1.0, 0.0, 0.0);
	osuVertex2f(x0,y1);
	osuColor3f(1.0, 0.0, 0.0);
	osuVertex2f(x2,y1);
	osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x1, y2);
  osuEnd ();

  x0 = 0.15;
  y0 = 0.55;
  x1 = 0.3;
  y1 = 0.7;
  x2 = 0.45;
  y2 = 0.85;

  osuBegin (OSU_TRIANGLE);
	osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x0, y1);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x1, y0);
	osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x2, y1);
  osuEnd();

  osuBegin(OSU_TRIANGLE);
  osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x0, y1);
	osuColor3f (1.0, 1.0, 1.0);
	osuVertex2f (x2, y1);
	osuColor3f (1.0, 0.0, 0.0);
	osuVertex2f (x1, y2);
  osuEnd ();

}

/******************************************************************************
Draw a polygonal approximation to a circle, but draw it with pie wedges
(thin triangles radiating from the center).

Entry:
  xc,yc  - circle center
  radius - radius of circle
  steps  - number of vertices in approximation
******************************************************************************/

void wedges (double xc, double yc, double radius, int steps)
{
  int i;
  double theta;
  double x,y;
  double xold,yold;

  theta = 2 * 3.1415926535 * (0.5) / (double) steps;
  xold = xc + radius * cos(theta);
  yold = yc + radius * sin(theta);

  for (i = 1; i <= steps; i++) {

    theta = 2 * 3.1415926535 * (i + 0.5) / (double) steps;
    x = xc + radius * cos(theta);
    y = yc + radius * sin(theta);

    osuBegin (OSU_TRIANGLE);
    osuVertex2f (xc, yc);
    osuVertex2f (x, y);
    osuVertex2f (xold, yold);
    osuEnd ();

    xold = x;
    yold = y;
  }
}

void overlapTest()
{
	  osuSetWriteMode(OSU_XOR);
	osuBegin(OSU_TRIANGLE);
	  osuColor3f(1.0,1.0,1.0);
	  osuVertex2f(0.1, 0.1);
	  osuVertex2f(0.6,0.5);
	  osuVertex2f(0.1,0.9);
	osuEnd();



	osuBegin(OSU_TRIANGLE);
		osuVertex2f(0.9, 0.1);
		osuVertex2f(0.4,0.5);
	    osuVertex2f(0.9, 0.9);
	osuEnd();
}
