#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "list.h"
#include "boid.h"
#include <glm/glm.hpp>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif

#define AVOIDANCE_WEIGHT          ((float) 0.8) // this should be significant as well
#define DEFAULT_OBSTACLE_WIDTH     5000
#define DEFAULT_OBSTACLE_HEIGHT    3000
#define DEFAULT_OBSTACLE_LENGTH    1000

const glm::vec4 OBSTACLE_DEFAULT_LOCATION  = glm::vec4(0,0,1000,1);
const float DEFAULT_AVOIDANCE_RANGE = 100.0;

const GLfloat OBSTACLE_VERTICES[][3] = { // to be adjusted according to default
                           {-100.0, -10.0, 100.0},  {-100.0, 100.0, 100.0}, 
                           {100.0, 10.00, 100.0},    {100.0, -100.0, 100.0}, 
                           {-100.0, -100.0, -100.0}, {-100.0, 100.0, -100.0}, 
                           {100.0, 100.0, -100.0},   {100.0, -100.0, -100.0}};

const GLfloat OBSTACLE_COLORS[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                                  {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, 
                                  {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, 
                                  {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};

const GLubyte OBSTACLE_INDICES[24] = {0, 3, 2, 1,
                                  2, 3, 7, 6,
                                  0, 4, 7, 3,
                                  1, 2, 6, 5,
                                  4, 5, 6, 7,
                                  0, 1, 5, 4};


typedef struct _obstacle{
  glm::vec4 pos;
  bool enable;
  float avoidance_range;
} OBSTACLE;

OBSTACLE* new_obstacle();
void toggle_obstable(OBSTACLE* an_obstacle);
void draw_a_goal(OBSTACLE* an_obstacle);

void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstable, float o_w);

#endif
