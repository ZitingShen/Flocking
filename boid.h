#ifndef BOID_H
#define BOID_H

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#define DEFAULT_FLOCKING_RADIUS   30.0
#define DEFAULT_FLOCK_SIZE        20
#define DEFAULT_SEPARATION_WEIGHT 0.3
#define DEFAULT_ALIGNMENT_WEIGHT  0.4
#define DEFAULT_COHESION_WEIGHT   0.5
#define DEFAULT_ATTRACTION_WEIGHT 0.1
#define DEFAULT_DETERRENCE_WEIGHT 1.0 // this should be the most significant weight
#define DEFUALT_AVOIDANCE_WEIGHT  0.8 // this should be significant as well

#define SPAWN_CUBE_LENGTH 50.0
const glm::vec4 SPAWN_POSITION = glm::vec4(0.0,0.0,0.0,1);
const glm::vec4 zero_vec = glm::vec4(0.0,0.0,0.0,0.0);
const glm::vec4 EMPTY_POS   = glm::vec4(0.0,0.0,0.0,1.0);
const glm::vec4 SPAWN_VELOCITY = glm::vec4(0,10.0,0,0); //initial speed parallel with y-axis

typedef struct _boid{
  //glm::mat4 left_rotation;       // TODO: for flapping extra credit       
  //glm::mat4 right_rotation;
  glm::vec4 pos;
  glm::vec4 velocity;              // also determines PA direction; and the degrees of rotation
  float partner_radius;           // the radius within which it looks for partners
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
BOID* new_boid(glm::vec4 velocity, float radius);
BOID* new_boid(glm::vec4 velocity, float radius, glm::vec4 pos);

bool is_partner(BOID* source, BOID* target);
void update_velocity(List* a_flock, float s_w, float a_w, float c_w);
void update_translation(List* a_flock);


glm::vec4 get_current_pos(BOID* a_boid);
glm::vec4 flock_centroid(List* a_flock);
glm::vec4 mid_point(List* a_flock, GOAL* a_goal);
glm::vec4 get_u(List* a_flock, GOAL* a_goal);
float get_d(List* a_flock, GOAL* a_goal);
float flock_radius(List* a_flock);

// assuming flocks are global variables, therefore no need to return
void add_a_boid(List* a_flock);
void remove_a_boid(List* a_flock);

void init_a_flock(List* a_flock);

void apply_goal_attraction(List* a_flock, GOAL* a_goal, float g_w);

/* To DO */
void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, float p_w);
void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstable, float o_w);
// Set speed cap as a global in main
#endif
