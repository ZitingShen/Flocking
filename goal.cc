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
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, CUBE_VERTICES);
  glColorPointer(3, GL_FLOAT, 0, CUBE_COLORS);
  glPushMatrix();
  glTranslatef(a_goal->pos.x, a_goal->pos.y, a_goal->pos.z);
  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, CUBE_INDICES);
  glPopMatrix();
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}
