#include "boid.h"

#include <stdio.h>
#include <stdlib.h>

bool is_partner(BOID* source, BOID* target){
  return source->flocking_radius>=
         glm::distance(source->pos, target->pos);
}

void update_velocity(List* a_flock, float s_w, float a_w, float c_w){
  NODE* current_boid=a_flock->head;
  NODE* potential_partner=a_flock->head;
  glm::vec4 s_modifier=zero_vec;
  glm::vec4 a_modifier=zero_vec;
  glm::vec4 c_modifier=zero_vec;
  int num_of_partners=0;
  while(current_boid!=NULL){
    while(potential_partner!=NULL && potential_partner!=current_boid){
        if(is_partner((BOID*)current_boid->data,
                      (BOID*)potential_partner->data)){
          num_of_partners++;
          s_modifier+=(((BOID*)(potential_partner->data))->pos
                               -((BOID*)(current_boid->data))->pos);//=target-source
          a_modifier += ((BOID*)(potential_partner->data))->velocity;
          c_modifier  += ((BOID*)(potential_partner->data))->pos;
        }
        potential_partner=potential_partner->next; //the next potential partner
    }
    s_modifier=s_w*(s_modifier/(float)num_of_partners);
    a_modifier=a_w*(a_modifier/(float)num_of_partners);
    c_modifier=c_w*(c_modifier/(float)num_of_partners);

    ((BOID*)(current_boid->data))->velocity
     =((BOID*)(current_boid->data))->velocity+s_modifier+a_modifier+c_modifier;

    current_boid=current_boid->next;//go on updating another boid
    potential_partner=a_flock->head;
    num_of_partners=0; //reset for the next boid
    s_modifier=zero_vec;
    a_modifier=zero_vec;
    c_modifier=zero_vec;
  }
}

void update_pos(List* a_flock){
  NODE* current=a_flock->head;
  while(current!=NULL){
     ((BOID*)(current->data))->pos+=((BOID*)(current->data))->velocity;
  }
}
