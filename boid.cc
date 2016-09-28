#include "boid.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

BOID* new_boid(glm::vec4 pos, glm::vec4 velocity, float radius){
  BOID* a_boid = new BOID;
  a_boid->pos = pos;
  a_boid->velocity = velocity;
  a_boid->flocking_radius = radius;
  return a_boid;
}

bool is_partner(BOID* source, BOID* target){
  return source->flocking_radius >=
         glm::distance(source->pos, target->pos);
}

void update_velocity(List* a_flock, float s_w, float a_w, float c_w){
  if (a_flock == NULL || a_flock->length == 0) return;
  NODE* current_boid = a_flock->head;
  NODE* potential_partner = a_flock->head;
  glm::vec4 s_modifier = zero_vec;
  glm::vec4 a_modifier = zero_vec;
  glm::vec4 c_modifier = zero_vec;
  int num_of_partners = 0;
  while (current_boid != NULL){
    while (potential_partner != NULL && potential_partner != current_boid){
        if (is_partner((BOID*)current_boid->data,
                      (BOID*)potential_partner->data)){
          num_of_partners++;
          s_modifier += (((BOID*)(potential_partner->data))->pos
                               -((BOID*)(current_boid->data))->pos);//=target-source
          a_modifier += ((BOID*)(potential_partner->data))->velocity;
          c_modifier += ((BOID*)(potential_partner->data))->pos;
        }
        potential_partner=potential_partner->next; //the next potential partner
    }
    s_modifier = s_w*(s_modifier/(float)num_of_partners);
    a_modifier = a_w*(a_modifier/(float)num_of_partners);
    c_modifier = c_w*(c_modifier/(float)num_of_partners);

    ((BOID*)(current_boid->data))->velocity
     = ((BOID*)(current_boid->data))->velocity+s_modifier+a_modifier+c_modifier;

    current_boid = current_boid->next;//go on updating another boid
    potential_partner = a_flock->head;
    num_of_partners = 0; //reset for the next boid
    s_modifier = zero_vec;
    a_modifier = zero_vec;
    c_modifier = zero_vec;
  }
}

void update_pos(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  NODE* current = a_flock->head;
  while(current != NULL){
     ((BOID*)(current->data))->pos += ((BOID*)(current->data))->velocity;
  }
}

glm::vec4 flock_centroid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return zero_vec;
  NODE* current = a_flock->head;
  glm::vec4 centroid = zero_vec;
  while (current != NULL){
     centroid += ((BOID*)(current->data))->pos;
     current=current->next;
  }
  return centroid*(float)((1.0f/(float)a_flock->length));
}

glm::vec4 mid_point(List* a_flock, GOAL* a_goal){
  if (a_flock == NULL || a_flock->length == 0) return zero_vec;
  return (flock_centroid(a_flock)+(a_goal->pos))*(0.5f);
}

glm::vec4 get_u(List* a_flock, GOAL* a_goal){
  if (a_flock == NULL || a_flock->length == 0) return zero_vec;
  return (a_goal->pos - flock_centroid(a_flock));
}

float get_d(List* a_flock, GOAL* a_goal){
  if (a_flock == NULL || a_flock->length == 0) return 0;
  return glm::distance(flock_centroid(a_flock), a_goal->pos);
}

float flock_radius(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return 0;

  float max_r = 0;
  float dis   = 0;
  NODE* current = a_flock->head;
  glm::vec4 centroid = flock_centroid(a_flock);
  while (current != NULL){
    dis = glm::distance(((BOID*)(current->data))->pos, centroid);
    max_r = max_r<dis?dis:max_r;
    current=current->next;
  }
  return max_r;
}

void add_a_boid(List* a_flock){
  if (a_flock == NULL){return;}
  BOID* target = (BOID*)list_get(a_flock, rand() % a_flock->length);

  glm::vec4 spawn_pos = target->pos;
  int modifer_upper_bond = static_cast<int>((target->flocking_radius)/(glm::sqrt(2)));
  for (int i=0; i<4; i++){
    srand(time(NULL));
    spawn_pos[i] = target->pos[i]
                   + (rand() % 2*modifer_upper_bond) - modifer_upper_bond;
  }
  list_insert(a_flock, new_boid(spawn_pos, target->velocity, target->flocking_radius), 0);

}

void remove_a_boid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return; //nothing to remove
  srand(time(NULL));
  list_delete(a_flock, rand() % a_flock->length);
}

void init_a_flock(List* a_flock, glm::vec4 pos, glm::vec4 v,
                   float flocking_radius, float cube_length, int num){
  if (a_flock == NULL){
    a_flock = new List;
  }

  glm::vec4 init_pos = zero_vec;
  float half_l = 0.5*cube_length;

  for (int i=0; i<num; i++){
    BOID* a_boid= new BOID;
    a_boid->velocity = v;
    a_boid->flocking_radius = flocking_radius;
    for (int j=0; j<4; j++){
      srand(time(NULL));
      init_pos[j] = pos[j] + (rand() % static_cast<int>(cube_length)) - half_l; //randomise the position
    }
    a_boid->pos = init_pos;
    list_insert(a_flock, a_boid, 0);
  }
}

void apply_goal_attraction(List* a_flock, GOAL* a_goal, float g_w){
  NODE* current=a_flock->head;
  glm::vec4 v_modifier = zero_vec;
  while (current!=NULL){
    v_modifier = g_w * (a_goal->pos - ((BOID*)(current->data))->pos);
    ((BOID*)(current->data))->velocity += v_modifier;
    current = current->next;
  }
}
