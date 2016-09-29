#ifndef GOAL_H
#define GOAL_H

#include <glm/glm.hpp>

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif

#define DEFAULT_ACCELERATION_FACTOR    0.01
#define DEFAULT_ACCELERATION_MARGIN   0.05
#define TRUE  1
#define FALSE 0

const glm::vec4 DEFAULT_GOAL_SPAWN_VELOCITY = glm::vec4(1,1,0.01,0); // initiated with a positive speed on Z-axis
const glm::vec4 DEFAULT_GOAL_SPAWN_POSITION = glm::vec4(1500,1500,1000,1);

const GLfloat CUBE_VERTICES[][3] = {
                           {-100.0, -10.0, 100.0},  {-100.0, 100.0, 100.0}, 
                           {100.0, 10.00, 100.0},    {100.0, -100.0, 100.0}, 
                           {-100.0, -100.0, -100.0}, {-100.0, 100.0, -100.0}, 
                           {100.0, 100.0, -100.0},   {100.0, -100.0, -100.0}};
const GLfloat CUBE_COLORS[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                                  {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, 
                                  {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, 
                                  {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
const GLubyte CUBE_INDICES[24] = {0, 3, 2, 1,
                                  2, 3, 7, 6,
                                  0, 4, 7, 3,
                                  1, 2, 6, 5,
                                  4, 5, 6, 7,
                                  0, 1, 5, 4};

typedef struct _goal{
  glm::vec4 pos;
  glm::vec4 velocity;
  bool MOVE_ALONG_X_POSITIVE;  // to control goal
  bool MOVE_ALONG_X_NEGATIVE;
  bool MOVE_ALONG_Y_POSITIVE;
  bool MOVE_ALONG_Y_NEGATIVE;
  bool ACCELERATE;
  bool DECELERATE;
} GOAL;


GOAL* new_goal();
void update_goal_velocity(GOAL* a_goal);
void update_goal_pos(GOAL* a_goal);
void draw_a_goal(GOAL* a_goal);

#endif
