#ifndef BOID_H
#define BOID_H

#include "list.h"
#include "goal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

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


#define RADIAN_TO_DEGREE          57.29578 // 180 over pi

#define PARTNER_RADIUS            100
#define DEFAULT_FLOCK_SIZE        20
#define DEFAULT_FLOCK_NUM         2

#define SEPARATION_WEIGHT         ((float) 0.02)
#define ALIGNMENT_WEIGHT          ((float) 0.01)
#define COHESION_WEIGHT           ((float) 0.01)
#define ATTRACTION_WEIGHT         ((float) 0.05)
#define DETERRENCE_WEIGHT         ((float) 0.5) // this should be the most significant weight
#define MAX_ATTRACTION_INFLUENCE  ((float) 10.0)

#define MAX_WING_ROTATION         45
#define WING_ROTATION_PER_FRAME   0.1

#define RANDOMISE_V_FACTOR        45
#define SPAWN_CUBE_LENGTH         100.0

const glm::vec4 SPAWN_POSITION_I  = glm::vec4(1000.0,1000.0,1500.0,1);
const glm::vec4 SPAWN_POSITION_II = glm::vec4(-1000.0,-1000.0, 500.0, 1);

const glm::vec4 ZERO_VEC = glm::vec4(0.0,0.0,0.0,0.0);
const glm::vec4 EMPTY_POS   = glm::vec4(0.0,0.0,0.0,1.0);
const glm::vec4 SPAWN_VELOCITY = glm::vec4(0,0.01,0,0); //initial speed parallel with y-axis

const float BOID_COLOUR_FLOCK_I[3] = {0.474, 0.118, 0.114};  // using burgundy for flock I
const float BOID_COLOUR_FLOCK_II[3] = {0.420, 0.792, 0.886}; // using aquamarine for flock II

#define BOID_SIZE                 100

const GLfloat A_BOID[][3] =
            {{0, 0, 0}, // position of the centroid
            {0, BOID_SIZE*2, 0}, // position of the head
            {-BOID_SIZE, -BOID_SIZE, 0}, // position of the left vertex
            {BOID_SIZE, -BOID_SIZE, 0}}; // position of the right vertex

const GLfloat A_BOID_COLORS[][3] = {
    {1.0, 1.0, 1.0}, 
    {BOID_COLOUR_FLOCK_I[0], BOID_COLOUR_FLOCK_I[1], BOID_COLOUR_FLOCK_I[2]},
    {1.0, 1.0, 1.0}, 
    {1.0, 1.0, 1.0}};

const GLfloat ANOTHER_BOID_COLORS[][3] = {
    {1.0, 1.0, 1.0}, 
    {BOID_COLOUR_FLOCK_II[0], BOID_COLOUR_FLOCK_II[1], BOID_COLOUR_FLOCK_II[2]},
    {1.0, 1.0, 1.0}, 
    {1.0, 1.0, 1.0}};

const glm::vec3 centroid_init = glm::vec3(0,0,0);
const glm::vec3 head_init = glm::vec3(0,BOID_SIZE*2,0);
const glm::vec3 left_init = glm::vec3(-BOID_SIZE,-BOID_SIZE,0);
const glm::vec3 right_init = glm::vec3(BOID_SIZE,-BOID_SIZE,0);

const GLubyte A_BOID_LEFT[3] = {0, 1, 2};
const GLubyte A_BOID_RIGHT[3] = {0, 3, 1}; //drawing two triangles;

typedef struct _boid{
  GLfloat wing_rotation;          // for flapping extra credit
  int wing_rotation_direction;
  int flock_index;  
  glm::vec4 pos;
  glm::vec4 velocity;             // also determines PA direction; and the degrees of rotation   
  float partner_radius;           // the radius within which it looks for partners
} BOID;

typedef struct _predator{
  glm::vec4 pos;
  glm::vec4 velocity;
  float deterrence_range;
  float attack_range;
} PREDATOR;

BOID* new_boid(glm::vec4 velocity, float radius);
BOID* new_boid(glm::vec4 velocity, float radius, glm::vec4 pos);

bool is_partner(BOID* source, BOID* target);
void update_velocity(List* a_flock);
void update_pos(List* a_flock);
void update_wing_rotation(List* a_flock);

glm::vec4 get_current_pos(BOID* a_boid);
glm::vec4 flock_centroid(List* a_flock);
glm::vec4 mid_point(List* a_flock, GOAL* a_goal);
glm::vec4 get_u(List* a_flock, GOAL* a_goal);
float get_d(List* a_flock, GOAL* a_goal);
float flock_radius(List* a_flock);

void add_a_boid(List* a_flock);
void remove_a_boid(List* a_flock);

void init_a_flock(List* a_flock);

void apply_goal_attraction(List* a_flock, GOAL* a_goal);
void draw_a_flock(List* a_flock);

void print_flock(List* a_flock);

glm::vec4 randomise_velocity(glm::vec4 raw_v);
//void update_rotation(BOID* a_boid);
/* To DO */
void apply_predator_deterrence(List* a_flock, PREDATOR* a_predator, float p_w);

// Set speed cap as a global in main
#endif
