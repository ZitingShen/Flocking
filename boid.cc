#include "boid.h"


BOID* new_boid(glm::vec4 velocity, float radius, float pos[3]){
  BOID* a_boid = new BOID;
  a_boid->translation = glm::make_mat4(identity_m);
  a_boid->translation[3][0] = pos[0];
  a_boid->translation[3][1] = pos[1];
  a_boid->translation[3][2] = pos[2];
  a_boid->init_pos = SPAWN_POSITION;    // the relative point to translation and rotation
  a_boid->velocity = velocity;
  a_boid->partner_radius = radius;
  return a_boid;
}

bool is_partner(BOID* source, BOID* target){
  return source->partner_radius >=
         glm::distance(glm::dot(source->translation, source->init_pos),
                       glm::dot(target->translation, target->init_pos);
}

void update_velocity(List* a_flock, float s_w, float a_w, float c_w){
  if (a_flock == NULL || a_flock->length == 0) return;
  NODE* current_boid = a_flock->head;
  NODE* potential_partner = a_flock->head;
  glm::vec4 s_modifier = zero_vec;
  glm::vec4 a_modifier = zero_vec;
  glm::vec4 c_modifier = zero_vec;
  int num_of_partners = 0;

  BOID* source = NULL;
  BOID* target = NULL;
  while (current_boid != NULL){
    while (potential_partner != NULL && potential_partner != current_boid){
        source = (BOID*)(current_boid->data);
        target = (BOID*)(potential_partner->data);
        if (is_partner(source, target)){
          num_of_partners++;
          s_modifier += (glm::dot(target->translation, target->init_pos)-
                         glm::dot(source->translation, source->init_pos));//=target-source
          a_modifier += target->velocity;
          c_modifier += glm::dot(target->translation, target->init_pos);
        }
        potential_partner=potential_partner->next; //the next potential partner
    }
    s_modifier = s_w*(s_modifier/(float)num_of_partners);
    a_modifier = a_w*(a_modifier/(float)num_of_partners);
    c_modifier = c_w*(c_modifier/(float)num_of_partners);

    source->velocity += (s_modifier+a_modifier+c_modifier);

    current_boid = current_boid->next;//go on updating another boid
    potential_partner = a_flock->head;
    num_of_partners = 0; //reset for the next boid
    s_modifier = zero_vec;
    a_modifier = zero_vec;
    c_modifier = zero_vec;
  }
}

void update_translation(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  glm::mat4 change = glm::make_mat4(identity_m);
  BOID* a_boid;
  NODE* current = a_flock->head;
  while (current != NULL){
    a_boid=((BOID*)(current->data));
    change[3][0] = a_boid->velocity[0];
    change[3][1] = a_boid->velocity[1];
    change[3][2] = a_boid->velocity[3];
    a_boid->translation = glm::dot(change, a_boid->translation);
    current=current->next;
  }
}

glm::vec4 get_current_pos(BOID* a_boid){
  return glm::dot(a_boid->translation, a_boid->init_pos); //order matters
}

glm::vec4 flock_centroid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return zero_vec;
  NODE* current = a_flock->head;
  glm::vec4 centroid = zero_vec;
  while (current != NULL){
     centroid += get_current_pos((BOID*)(current->data));
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
    dis = glm::distance(get_current_pos((BOID*)(current->data)), centroid);
    max_r = max_r<dis?dis:max_r;
    current=current->next;
  }
  return max_r;
}

void add_a_boid(List* a_flock){
  if (a_flock == NULL){return;}  // use init_a_flock to create a new flock
  BOID* target = (BOID*)list_get(a_flock, rand() % a_flock->length);
  float new_pos[3];
  // spwaning within the partner radius of the target
  int modifier_upper_bond = static_cast<int>((target->partner_radius)/(glm::sqrt(2)));
  int half_moifdier       = static_cast<int>(modifier_upper_bond);
  for (int i=0; i<3; i++){
    srand(time(NULL));
    new_pos[i] = target->translation[3][i]
               + (rand() % half_moifdier) - modifier_upper_bond;
  }

  list_insert(a_flock, new_boid(target->velocity, target->partner_radius, new_pos), 0);
}

void remove_a_boid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return; //nothing to remove
  srand(time(NULL));
  list_delete(a_flock, rand() % a_flock->length);
}

void init_a_flock(List* a_flock, glm::vec4 v,
                   float partner_radius, float cube_length, int num){
  if (a_flock == NULL){
    a_flock = new List;
  }

  float half_l = 0.5*cube_length;

  for (int i=0; i<num; i++){
    BOID* a_boid= new BOID;
    a_boid->init_pos = SPAWN_POSITION;
    a_boid->velocity = v;
    a_boid->partner_radius = partner_radius;
    for (int j=0; j<3; j++){
      srand(time(NULL));
      a_boid->translation = glm::make_mat4(identity_m);
      a_boid->translation[3][i] =
          a_boid->init_pos[j] + (rand() % static_cast<int>(half_l)) - cube_length; //randomise the position
    }
    list_insert(a_flock, a_boid, 0);
  }
}

void apply_goal_attraction(List* a_flock, GOAL* a_goal, float g_w){
  NODE* current=a_flock->head;
  glm::vec4 v_modifier = zero_vec;
  while (current!=NULL){
    v_modifier = g_w * (a_goal->pos - get_current_pos((BOID*)(current->data)));
    ((BOID*)(current->data))->velocity += v_modifier;
    current = current->next;
  }
}
