#include "boid.h"
#include <iostream>

BOID* new_boid(glm::vec4 velocity, float radius){
  BOID* a_boid = new BOID;
  a_boid->pos = SPAWN_POSITION;
  a_boid->velocity = velocity;
  a_boid->partner_radius = radius;
  return a_boid;
}

BOID* new_boid(glm::vec4 velocity, float radius, glm::vec4 pos){
  BOID* a_boid = new BOID;
  a_boid->pos = pos;
  a_boid->velocity = velocity;
  a_boid->partner_radius = radius;
  return a_boid;
}

bool is_partner(BOID* source, BOID* target){
  return source->partner_radius >=
         glm::distance(source->pos, target->pos);
}

void update_velocity(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  NODE* current_boid = a_flock->head;
  NODE* potential_partner;
  glm::vec4 s_modifier, a_modifier, c_modifier;
  int num_of_partners;

  BOID* source = NULL;
  BOID* target = NULL;
  while (current_boid != NULL){
    num_of_partners = 0; //reset for the next boid
    s_modifier = zero_vec;
    a_modifier = zero_vec;
    c_modifier = zero_vec;
    potential_partner = a_flock->head;
    num_of_partners = 0;
    while (potential_partner != NULL){
      if (potential_partner == current_boid) {
        potential_partner = potential_partner->next;
        continue;
      }
      source = (BOID*)(current_boid->data);
      target = (BOID*)(potential_partner->data);
      if (is_partner(source, target)){
        num_of_partners++;
        s_modifier += source->pos - target->pos;
        a_modifier += target->velocity;
        c_modifier += target->pos;
      }
      potential_partner=potential_partner->next;
    }
    if (num_of_partners != 0) {
      s_modifier = SEPARATION_WEIGHT*(s_modifier/(float)num_of_partners);
      a_modifier = ALIGNMENT_WEIGHT*(a_modifier/(float)num_of_partners - source->velocity);
      c_modifier = COHESION_WEIGHT*(c_modifier/(float)num_of_partners - source->pos);
    }

    source->velocity += (s_modifier+a_modifier+c_modifier);
    current_boid = current_boid->next;
  }
}

void update_pos(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return;
  BOID* a_boid;
  NODE* current = a_flock->head;
  while (current != NULL){
    a_boid = (BOID*)(current->data);
    a_boid->pos += a_boid->velocity*((float)0.01);
    current = current->next;
  }
}

glm::vec4 get_current_pos(BOID* a_boid){
  return a_boid->pos; //order matters
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
  //return (flock_centroid(a_flock)+(a_goal->pos));
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
  int default_cube_length = PARTNER_RADIUS/glm::sqrt(2);
  int half_cube_length    = default_cube_length/2;
  glm::vec4 pos;
  if (a_flock->length == 0) {
    glm::vec4 pos;
    pos.x = (rand() % default_cube_length) - half_cube_length; 
    pos.y = (rand() % default_cube_length) - half_cube_length; 
    pos.z = (rand() % default_cube_length) - half_cube_length;
    list_insert(a_flock, new_boid(SPAWN_VELOCITY, PARTNER_RADIUS, pos), 0);
    return;
  }

  BOID* target = (BOID*)list_get(a_flock, rand() % a_flock->length);
  // spawning within the partner radius of the target
  pos.x = target->pos.x + (rand() % default_cube_length) - half_cube_length; 
  pos.y = target->pos.y + (rand() % default_cube_length) - half_cube_length; 
  pos.z = target->pos.z + (rand() % default_cube_length) - half_cube_length; 
  pos.w = 1;

  list_insert(a_flock, new_boid(target->velocity, target->partner_radius, pos), 0);
}

void remove_a_boid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return; //nothing to remove
  srand(time(NULL));
  list_delete(a_flock, rand() % a_flock->length);
}

void init_a_flock(List* a_flock){
  int default_cube_length = SPAWN_CUBE_LENGTH;
  int half_cube_length = default_cube_length/2;

  for (int i = 0; i < DEFAULT_FLOCK_SIZE; i++){
    BOID* a_boid= new BOID;
    a_boid->pos = SPAWN_POSITION;
    a_boid->velocity = SPAWN_VELOCITY;
    a_boid->partner_radius = PARTNER_RADIUS;
    a_boid->pos.x += (rand() % default_cube_length) - half_cube_length;
    a_boid->pos.y += (rand() % default_cube_length) - half_cube_length;
    a_boid->pos.z += (rand() % default_cube_length) - half_cube_length;
    list_insert(a_flock, a_boid, 0);
  }
}

void draw_a_flock(List* a_flock){
  if (a_flock == NULL) return;
  NODE* current = a_flock->head;
  BOID* some_boid = NULL;

  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0.0, 0.0, 0.0);
  glVertexPointer(3, GL_FLOAT, 0, A_BOID);

  for (int i = 0; i < a_flock->length; i++){
    some_boid = (BOID*)(current->data);
    
    glm::vec4 rotate_normal = glm::normalize(some_boid->velocity);
    GLfloat rotate_matrix[16] = {1, 0, 0, rotate_normal.x,
                                 0, 1, 0, rotate_normal.y,
                                 0, 0, 1, rotate_normal.z,
                                 0, 0, 0, 1};
    //if (i == 0) {
    //  std::cout << some_boid->pos.x << " " << some_boid->pos.y << " " << some_boid->pos.z << std::endl;
    //  glm::vec4 midpoint = mid_point(A_FLOCK, &A_GOAL);
    //  std::cout << midpoint.x << " " << midpoint.y << " " << midpoint.z << std::endl;
    //}
    glPushMatrix();
    glTranslatef(some_boid->pos.x, some_boid->pos.y, some_boid->pos.z);
    //glMultMatrixf(rotate_matrix);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, A_BOID_VERTICES);
    glPopMatrix();
    current = current->next;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
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
