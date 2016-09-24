#ifndef BOID_H
#define BOID_H

#include "list.h"
#include <glm/glm.hpp>

const glm::vec4 zero_vec=glm::vec4(0.0,0.0,0.0,0.0);

typedef struct _boid{
  glm::vec4 pos;           // [x_pos, y_pos, z_pos]
  glm::vec4 velocity;      // [s*cos(theta), s*sin(theta), 0]
  float flocking_radius;   // the radius within which it looks for partners
} BOID;

bool is_partner(BOID* source, BOID* target);
void update_velocity(List* a_flock, float s_w, float a_w, float c_w);
void update_pos(List* a_flock);

// Set speed cap as a global in main

#endif
