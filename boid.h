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

typedef struct _predator{
  glm::vec4 pos;
  glm::vec4 velocity;
  float deterrence_range;
  float attack_range;
} PREDATOR;

typedef struct _obstacle{
  glm::vec4 pos;
  float avoidance_range;
} OBSTACLE;

BOID* new_boid(glm::vec4 pos, glm::vec4 velocity, float radius);

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

void init_a_flock(List* a_flock, glm::vec4 pos, glm::vec4 velocity,
                  float flocking_radius, float cube_length, int num);

void apply_goal_attraction(List* a_flock, GOAL* a_goal, float g_w);

/* To DO */
void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, float p_w);
void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstable, float o_w);
// Set speed cap as a global in main
#endif
