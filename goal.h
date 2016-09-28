#ifndef GOAL_H
#define GOAL_H

#include <glm/glm.hpp>

typedef struct _goal{
  glm::vec4 pos;
  glm::vec4 velocity;
int MOVE_ALONG_X_AXIS  = 0;  // to control goal
int MOVE_ALONG_Y_AXIS  = 0;
int MOVE_ALONG_Z_AXIS  = 0;
int POSITIVE_DIRECTION = 0;
} GOAL;

#endif
