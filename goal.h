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

#define DEFAULT_ACCELERATION 1
#define TRUE  1
#define FALSE 0

const glm::vec4 DEFAULT_GOAL_SPAWN_VELOCITY = glm::vec4(0,0,10,0); // initiated with a positive speed on Z-axis
const glm::vec4 DEFAULT_GOAL_SPAWN_POSITION = glm::vec4(10,10,10,1);

typedef struct _goal{
  glm::vec4 pos;
  glm::vec4 velocity;
  bool MOVE_ALONG_X_POSITIVE;  // to control goal
  bool MOVE_ALONG_X_NEGATIVE;
  bool MOVE_ALONG_Y_POSITIVE;
  bool MOVE_ALONG_Y_NEGATIVE;
} GOAL;


GOAL* new_goal();
void update_goal_velocity(GOAL* a_goal);
void draw_a_goal(GOAL* a_goal);

#endif
