#include "obstacle.h"

OBSTACLE* new_obstacle(){
	OBSTACLE* an_obstacle = (OBSTACLE*)malloc(sizeof(OBSTACLE));
	an_obstacle->pos = OBSTACLE_DEFAULT_LOCATION;
	an_obstacle->enable = true;
	an_obstacle->avoidance_range = DEFAULT_AVOIDANCE_RANGE;
	return an_obstacle;
}

void toggle_obstable(OBSTACLE* an_obstacle){
	an_obstacle->enable = !an_obstacle->enable;
}

/*
void draw_a_goal(OBSTACLE* an_obstacle){
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
*/

void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstacle){
  if(a_flock == NULL || an_obstacle == NULL) return;
  NODE* current = a_flock->head;
  BOID* a_boid = (BOID*)(current->data);
  glm::vec3 boid_pos = glm::vec3(a_boid->pos[0], a_boid->pos[1], a_boid->pos[2]);
  glm::vec3 obstacle_unit_pos;
  glm::vec3 imminence = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
  while(current != NULL){
    for (int i = 0; i<DEFAULT_OBSTACLE_WIDTH; i++){
      for (int j = 0; j<DEFAULT_OBSTACLE_HEIGHT; j++){
        for (int k = 0; k<DEFAULT_OBSTACLE_LENGTH; k++){
         obstacle_unit_pos = glm::vec3(i+an_obstacle->pos[0],
                                       j+an_obstacle->pos[1],
                                       k+an_obstacle->pos[2]);
         imminence = (glm::length(boid_pos - obstacle_unit_pos)
                     < glm::length(imminence))? 
        }
      }
  }
}
