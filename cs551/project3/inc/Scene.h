#ifndef SCENE_H
#define SCENE_H

#include "osuGraphics.h"
#include "ObjLoader.h"
#include "Vector.h"
#include <vector>

void init_scene(void);
void destroy_scene(void);

void begin_scene(Material &m, std::vector<PLight> _p_lights, std::vector<DLight> _d_lights);
void draw_model(ObjModel &model);
void end_scene(void);

#endif
