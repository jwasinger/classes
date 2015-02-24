#include <iostream>
#include "osuGraphics.h"
#include "objLoader.h"
#include <string>
#include <vector>

std::string model_path = ".\\media\\models\\";

ObjLoader *loader;
ObjModel model;


void sphere_scene(void)
{
  loader = new ObjLoader();
  loader->LoadObj(model_path + "sphere.obj");
  model = loader->ReturnObj();

  std::vector<DLight> d_lights;
  std::vector<PLight> p_lights;
  Material m = 
  {
    0.5f,
    Vector4(1.0, 0.0, 0.0), //red diffuse light
    Vector4(1.0, 1.0, 1.0, 0.7); //white specular highlights
  };
  
  begin_scene(m, d_lights, p_lights);
  draw_model(model);
  end_scene();
  
  loader->FreeObj();
  delete loader;
}

void cube_scene(void)
{
      
}

int main(int argc, char **argv)
{
  int num = atoi(argv[1]);

  if((num <0 ) || (num > 11))
  {
	fprintf(stderr, "Please call this program with a number from 1 to 12 \n");
	exit(-1);
  }

  A2Math::Matrix::matrix_tests();

  osuBeginGraphics (xsize, ysize);

  /* inialize the matrix stack*/
  osuInitialize(); 
  
  switch(num)
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
  }

  osuFlush();

  printf ("Press 'escape' to exit.\n");
  osuWaitOnEscape();
  osuEndGraphics();
 
}
