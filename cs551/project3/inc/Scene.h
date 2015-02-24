#ifndef SCENE_H
#define SCENE_H

#include "osuGraphics.h"
#include "Vector.h"

struct Material
{
  float ambient_i;
  Vector4 diffuse; // w-component unused
  Vector4 specular; // w-component used for Phong coefficient
};

Material NULLMaterial = 
{
  0.0, 
  Vector4(0.0, 0.0, 0.0, 0.0),
  Vector4(0.0, 0.0, 0.0, 0.0)
};

struct PLight
{
  Vector4 pos_i;
}

struct DLight
{
  Vector4 dir_i;
}


void init_scene(void);
void destroy_scene(void);

void begin_scene(Material &m, std::vector<PLight> _p_lights, std::vector<DLight> _d_lights);
void draw_model(ObjModel &model);
void end_scene(void);


#endif
