#include "obstacle.h"

OBSTACLE* new_obstacle(){
	OBSTACLE* an_obstacle = (OBSTACLE*)malloc(sizeof(OBSTACLE));
	an_obstacle->pos = OBSTACLE_DEFAULT_LOCATION;
	an_obstacle->enable = false;
	an_obstacle->avoidance_range = DEFAULT_AVOIDANCE_RANGE;
	return an_obstacle;
}

void toggle_obstable(OBSTACLE* an_obstacle){
	an_obstacle->enable = !an_obstacle->enable;
}

void draw_an_obstacle(OBSTACLE* an_obstacle){
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, OBSTACLE_VERTICES);
  glColorPointer(3, GL_FLOAT, 0, OBSTACLE_COLORS);
  glPushMatrix();
  glTranslatef(an_obstacle->pos.x, an_obstacle->pos.y, an_obstacle->pos.z);
  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, OBSTACLE_INDICES);
  glPopMatrix();
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstacle){
  if(a_flock == NULL || an_obstacle == NULL || !an_obstacle->enable) return;
  NODE* current = a_flock->head;
  BOID* a_boid = NULL;
  glm::vec3 obstacle_unit_pos;
  glm::vec3 imminence = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
  glm::vec3 detection_vec;
  glm::vec3 temp_v;
  glm::vec4 avoidance_vec;
  while(current != NULL){
    a_boid = (BOID*)(current->data);
    glm::vec3 boid_pos = glm::vec3(a_boid->pos[0], a_boid->pos[1], a_boid->pos[2]);
    for (int i = 0; i<DEFAULT_OBSTACLE_WIDTH; i+=100){
      for (int j = 0; j<DEFAULT_OBSTACLE_HEIGHT; j+=100){
        for (int k = 0; k<DEFAULT_OBSTACLE_LENGTH; k+=100){
          if(!(i==0||j==0||k==0||
               i==DEFAULT_OBSTACLE_WIDTH-1||j==DEFAULT_OBSTACLE_HEIGHT-1||k==DEFAULT_OBSTACLE_LENGTH-1)){
            break; // only to calculate if this point is on the surface of obstacles
          }
          obstacle_unit_pos = glm::vec3(i+an_obstacle->pos[0],
                                        j+an_obstacle->pos[1],
                                        k+an_obstacle->pos[2]);
          detection_vec = obstacle_unit_pos - boid_pos;
          imminence = (glm::length(detection_vec)< glm::length(imminence))?
                       detection_vec:imminence;
        }
      }
    }
    if (an_obstacle->avoidance_range > glm::length(imminence)){ //only avoid when in range
      temp_v = glm::vec3(a_boid->velocity[0],a_boid->velocity[1],a_boid->velocity[2]);
      avoidance_vec = glm::vec4(glm::cross(imminence, temp_v),0);
      a_boid->velocity += (AVOIDANCE_WEIGHT)*avoidance_vec; //more immient, more weight
    }
    current = current->next;
  }
}
