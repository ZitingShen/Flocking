#include "goal.h"

GOAL* new_goal(){
	GOAL* a_goal = new GOAL;
	a_goal->pos = DEFAULT_GOAL_SPAWN_POSITION;
  a_goal->velocity = DEFAULT_GOAL_SPAWN_VELOCITY;
  a_goal->MOVE_ALONG_X_NEGATIVE = false;
  a_goal->MOVE_ALONG_X_POSITIVE = false;
  a_goal->MOVE_ALONG_Y_NEGATIVE = false;
  a_goal->MOVE_ALONG_Y_POSITIVE = false;
  return a_goal;
}

void update_goal_velocity(GOAL* a_goal){
  if (a_goal->MOVE_ALONG_X_POSITIVE)
    a_goal->velocity[0] += (float)DEFAULT_ACCELERATION;
  if (a_goal->MOVE_ALONG_X_NEGATIVE)
    a_goal->velocity[0] -= (float)DEFAULT_ACCELERATION;
  if (a_goal->MOVE_ALONG_Y_POSITIVE)
    a_goal->velocity[1] += (float)DEFAULT_ACCELERATION;
  if (a_goal->MOVE_ALONG_Y_NEGATIVE)
    a_goal->velocity[1] -= (float)DEFAULT_ACCELERATION;
}

void update_goal_pos(GOAL* a_goal){
  a_goal->pos += a_goal->velocity;
}

void draw_a_goal(GOAL* a_goal){


}
