#ifndef BOID_H
#define BOID_H

#include "list.h"
#include "goal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#define PARTNER_RADIUS            20
#define DEFAULT_FLOCK_SIZE        20

#define SEPARATION_WEIGHT         ((float) 0.005)
#define ALIGNMENT_WEIGHT          ((float) 0.002)
#define COHESION_WEIGHT           ((float) 0.006)
#define ATTRACTION_WEIGHT         ((float) 0.1)
#define DETERRENCE_WEIGHT         ((float) 1.0) // this should be the most significant weight
#define AVOIDANCE_WEIGHT          ((float) 0.8) // this should be significant as well

#define RANDOMISE_V_FACTOR        45
#define SPAWN_CUBE_LENGTH         50.0
const glm::vec4 SPAWN_POSITION = glm::vec4(0.0,0.0,100.0,1);
const glm::vec4 zero_vec = glm::vec4(0.0,0.0,0.0,0.0);
const glm::vec4 EMPTY_POS   = glm::vec4(0.0,0.0,0.0,1.0);
const glm::vec4 SPAWN_VELOCITY = glm::vec4(0,10.0,0,0); //initial speed parallel with y-axis

#define BOID_SIZE                 5
//const GLfloat A_BOID[][3] =
//            {{0, 0, 0}, // position of the centroid
//            {0, BOID_SIZE*2, 0}, // position of the head
//            {-BOID_SIZE, -BOID_SIZE, 0}, // position of the left vertex
//            {BOID_SIZE, -BOID_SIZE, 0}}; // position of the right vertex;

const glm::vec3 centroid_init = glm::vec3(0,0,0);
const glm::vec3 head_init = glm::vec3(0,BOID_SIZE*2,0);
const glm::vec3 left_init = glm::vec3(-BOID_SIZE,-BOID_SIZE,0);
const glm::vec3 right_init = glm::vec3(BOID_SIZE,-BOID_SIZE,0);


const GLubyte A_BOID_VERTICES[6] = {0, 1, 2,
                                    0, 3, 1}; //drawing two triangles;

extern GLfloat A_BOID[][3]; //to be defined in boid.cc


typedef struct _boid{
  //glm::mat4 left_rotation;       // TODO: for flapping extra credit       
  //glm::mat4 right_rotation;
  glm::vec4 pos;
  glm::vec4 velocity;              // also determines PA direction; and the degrees of rotation
  float partner_radius;           // the radius within which it looks for partners
} BOID;

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
void update_velocity(List* a_flock);
void update_pos(List* a_flock);


glm::vec4 get_current_pos(BOID* a_boid);
glm::vec4 flock_centroid(List* a_flock);
glm::vec4 mid_point(List* a_flock, GOAL* a_goal);
glm::vec4 get_u(List* a_flock, GOAL* a_goal);
float get_d(List* a_flock, GOAL* a_goal);
float flock_radius(List* a_flock);

void add_a_boid(List* a_flock);
void remove_a_boid(List* a_flock);

void init_a_flock(List* a_flock);

void apply_goal_attraction(List* a_flock, GOAL* a_goal, float g_w);
void draw_a_flock(List* a_flock);

glm::vec4 randomise_velocity(glm::vec4 raw_v);
void update_rotation(BOID* a_boid);
/* To DO */
void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, float p_w);
void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstable, float o_w);
// Set speed cap as a global in main
#endif
