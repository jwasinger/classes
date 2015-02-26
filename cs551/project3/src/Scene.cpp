#include "Scene.h"
#include <vector>

static std::vector<DLight>  d_lights;
static std::vector<PLight> p_lights;
static Material mat;

void init_scene(void)
{

}

void destroy_scene(void)
{

}

void begin_scene(Material &m, std::vector<DLight> _d_lights, std::vector<PLight> _p_lights)
{
  d_lights = _d_lights;
  p_lights = _p_lights;
  mat = m;
}

void draw_model(ObjModel &model)
{
  ObjVertex v0, v1, v2;
  ObjNormal n0, n1, n2;

  for(int i = 0; i < model.NumTriangle; i++)
  { 
    osuBegin(OSU_TRIANGLE);
    
    v0 = model.VertexArray[model.TriangleArray[i].Vertex[0]];
    v1 = model.VertexArray[model.TriangleArray[i].Vertex[1]]; 
    v2 = model.VertexArray[model.TriangleArray[i].Vertex[2]]; 
    
    n0 = model.NormalArray[model.TriangleArray[i].Normal[0]];
    n1 = model.NormalArray[model.TriangleArray[i].Normal[1]];
    n2 = model.NormalArray[model.TriangleArray[i].Normal[2]];
    
	
    osuNormal3f((double)n0.X, (double)n0.Y, (double)n0.Z); 
    osuVertex3f((double)v0.X, (double)v0.Y, (double)v0.Z);

    osuNormal3f((double)n1.X, (double)n1.Y, (double)n1.Z);
    osuVertex3f((double)v1.X, (double)v1.Y, (double)v1.Z);
 
    osuNormal3f((double)n2.X, (double)n2.Y, (double)n2.Z);
    osuVertex3f((double)v2.X, (double)v2.Y, (double)v2.Z);
	
	osuFaceNormal3f((double)model.TriangleArray[i].faceNormal[0],
					(double)model.TriangleArray[i].faceNormal[1],
					(double)model.TriangleArray[i].faceNormal[2]);

    osuEnd();
  }
}

void end_scene(void)
{
  d_lights = std::vector<DLight>(0);
  p_lights = std::vector<PLight>(0);
  mat = NULLMaterial;
}
