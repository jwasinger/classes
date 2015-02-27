#include <iostream>
#include "Scene.h"
#include <string>
#include <vector>

std::string model_path = ".\\media\\models\\";

ObjLoader *loader;
ObjModel model;

void cube_scene(void)
{
      
}

void sphere_scene(void)
{
	loader = new ObjLoader();
	loader->LoadObj(model_path + "cube.obj");
	model = loader->ReturnObj();

	std::vector<DLight> d_lights;
	d_lights.push_back({ Vector4(1.0, 1.0, -1.0) });
	
	std::vector<PLight> p_lights;
	Material m =
	{
		0.5f,
		Vector4(1.0, 0.0, 0.0), //red diffuse light
		Vector4(1.0, 1.0, 1.0, 0.7) //white specular highlights
	};

	begin_scene(m, p_lights, d_lights);

	Matrix proj = Matrix::CreatePerspectiveFOV(3.14159 / 2.0, 1.0, 1.0, 100.0);
	Matrix eye = Matrix::CreateLookAt(Vector4(0.0f, 0.0f, -1.0f), Vector4(0.0, 0.0, 0.0), Vector4(0.0, 1.0, 0.0));
	Matrix tr = Matrix::CreateTranslation(Vector4(0.0, 0.0, 100.0));
	
	mat_stack.set_proj(proj);
	mat_stack.push_mat(tr);
	mat_stack.push_mat(eye);
	
	draw_model(model);
	end_scene();

	delete loader;
}

int main(int argc, char **argv)
{
  /*int num = atoi(argv[1]);

  if((num <0 ) || (num > 11))
  {
	fprintf(stderr, "Please call this program with a number from 1 to 12 \n");
	exit(-1);
  }*/

  int xsize = 360;
  int ysize = 360;

  osuBeginGraphics (xsize, ysize);

  /* inialize the matrix stack*/
  osuInitialize(); 
  
  sphere_scene();
  /*switch(num)
  {
    case 0:
      
      break;
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
  }*/

  osuFlush();

  printf ("Press 'escape' to exit.\n");
  osuWaitOnEscape();
  osuEndGraphics();
 
}
