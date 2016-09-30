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

void apply_obstacle_avoidance(List* a_flock, OBSTACLE* an_obstacle){
  if(a_flock == NULL || an_obstacle == NULL) return;
  NODE* current = a_flock->head;
  BOID* a_boid = (BOID*)(current->data);
  glm::vec3 boid_pos = glm::vec3(a_boid->pos[0], a_boid->pos[1], a_boid->pos[2]);
  glm::vec3 obstacle_unit_pos;
  glm::vec3 imminence = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
  glm::vec3 detection_vec, imminent_threat;
  glm::vec3 temp_v;
  glm::vec4 avoidance_vec;
  while(current != NULL){
    for (int i = 0; i<DEFAULT_OBSTACLE_WIDTH; i++){
      for (int j = 0; j<DEFAULT_OBSTACLE_HEIGHT; j++){
        for (int k = 0; k<DEFAULT_OBSTACLE_LENGTH; k++){
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
          temp_v = glm::vec3(a_boid->velocity[0],a_boid->velocity[1],a_boid->velocity[2]);
          avoidance_vec = glm::vec4(glm::cross(imminent_threat, temp_v),0);
        }
      }
  }
}
