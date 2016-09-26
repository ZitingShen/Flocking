#ifndef BOID_H
#define BOID_H

#include "list.h"
#include <glm/glm.hpp>

const glm::vec4 zero_vec = glm::vec4(0.0,0.0,0.0,0.0);

typedef struct _boid{
  glm::vec4 pos;           // [x_pos, y_pos, z_pos]
  glm::vec4 velocity;      // [s*cos(theta), s*sin(theta), 0]
  float flocking_radius;   // the radius within which it looks for partners
} BOID;

typedef struct _goal{
  glm::vec4 pos;
  glm::vec4 velocity;
} GOAL;

BOID* new_boid(glm::vec4 p, glm::vec4 v, float r);

bool is_partner(BOID* source, BOID* target);
void update_velocity(List* a_flock, float s_w, float a_w, float c_w);
void update_pos(List* a_flock);

glm::vec4 flock_centroid(List* a_flock);
glm::vec4 mid_point(List* a_flock, GOAL* a_goal);
glm::vec4 get_u(List* a_flock, GOAL* a_goal);
float get_d(List* a_flock, GOAL* a_goal);
float flock_radius(List* a_flock);

// assuming flocks are global variables, therefore no need to return
void add_a_boid(List* a_flock);
void remove_a_boid(List* a_flock);

void init_a_flock(List* a_flock, glm::vec4 pos, glm::vec4 v, float f_r,
                  float l, int num);

// Set speed cap as a global in main
#endif
