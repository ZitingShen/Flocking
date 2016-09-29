#include "boid.h"
#include <iostream>
/*
GLfloat A_BOID[][3] = {{0,0,0},
                       {0,0,0},
                       {0,0,0},
                       {0,0,0}};
*/
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
  if (a_flock == NULL || a_flock->length == 0) 
    return glm::vec4(0, 0, 0, 1);
  NODE* current = a_flock->head;
  glm::vec4 centroid = glm::vec4(0, 0, 0, 1);
  while (current != NULL){
     centroid += get_current_pos((BOID*)(current->data));
     current = current->next;
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
    dis = glm::distance(get_current_pos((BOID*) (current->data)), centroid);
    max_r = max_r < dis ? dis : max_r;
    current = current->next;
  }
  return max_r;
}

void add_a_boid(List* a_flock){
  if (a_flock == NULL){return;}  // use init_a_flock to create a new flock
  int default_cube_length = PARTNER_RADIUS*glm::sqrt(2);
  int half_cube_length    = default_cube_length/2;
  glm::vec4 pos;
  if (a_flock->length == 0) {
    glm::vec4 pos;
    pos.x = (rand() % default_cube_length) - half_cube_length;
    pos.y = (rand() % default_cube_length) - half_cube_length;
    pos.z = (rand() % default_cube_length) - half_cube_length;
    pos.w = 1;
    list_insert(a_flock, new_boid(SPAWN_VELOCITY, PARTNER_RADIUS, pos), 0);
    return;
  }

  BOID* target = (BOID*)list_get(a_flock, rand() % a_flock->length);
  // spawning within the partner radius of the target
  pos.x = target->pos.x + (rand() % default_cube_length) - half_cube_length;
  pos.y = target->pos.y + (rand() % default_cube_length) - half_cube_length;
  pos.z = target->pos.z + (rand() % default_cube_length) - half_cube_length;
  pos.w = 1;

  list_insert(a_flock, new_boid(randomise_velocity(target->velocity),
                                PARTNER_RADIUS, pos), 0);
}

void remove_a_boid(List* a_flock){
  if (a_flock == NULL || a_flock->length == 0) return; //nothing to remove
  list_delete(a_flock, rand() % a_flock->length);
}

void init_a_flock(List* a_flock){
  int default_cube_length = SPAWN_CUBE_LENGTH*glm::sqrt(2);
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

  glColor3f(0.0, 0.0, 0.0);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, A_BOID);

  for (int i = 0; i < a_flock->length; i++){
    some_boid = (BOID*)(current->data);
    //update_rotation(some_boid);
    //glVertexPointer(3, GL_FLOAT, 0, A_BOID);
    glm::vec3 velocity3(some_boid->velocity);
    glm::vec3 initial3(SPAWN_VELOCITY);
    glm::vec3 rotate_normal = glm::normalize(glm::cross(velocity3, initial3));
    float angle = glm::orientedAngle(initial3, velocity3, 
                                      rotate_normal);

    glPushMatrix();
    glTranslatef(some_boid->pos.x, some_boid->pos.y, some_boid->pos.z);
    glRotatef(angle, rotate_normal.x, rotate_normal.y, rotate_normal.z);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, A_BOID_VERTICES);
    glPopMatrix();
    current = current->next;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}

void apply_goal_attraction(List* a_flock, GOAL* a_goal){
  NODE* current=a_flock->head;
  glm::vec4 v_modifier = zero_vec;
  while (current!=NULL){
    v_modifier = ATTRACTION_WEIGHT*(a_goal->pos - get_current_pos((BOID*)(current->data)));
    ((BOID*)(current->data))->velocity += v_modifier;
    current = current->next;
  }
}

glm::vec4 randomise_velocity(glm::vec4 raw_v){
  glm::vec4 new_velocity;

  new_velocity = glm::rotateX(raw_v,
                       (float)(rand()%(2*RANDOMISE_V_FACTOR)-RANDOMISE_V_FACTOR));

  new_velocity = glm::rotateY(raw_v,
                       (float)(rand()%(2*RANDOMISE_V_FACTOR)-RANDOMISE_V_FACTOR));

  new_velocity = glm::rotateZ(raw_v,
                       (float)(rand()%(2*RANDOMISE_V_FACTOR)-RANDOMISE_V_FACTOR));

  return new_velocity;
}
/*
void update_rotation(BOID* a_boid){
  glm::vec3 new_head = glm::vec3(a_boid->velocity[0],a_boid->velocity[1],a_boid->velocity[2]);
  glm::vec3 x_axis_ref = glm::vec3(1,0,0);
  glm::vec3 y_axis_ref = glm::vec3(1,0,0);
  glm::vec3 z_axis_ref = glm::vec3(0,0,1); //using z-aixs as the reference angle
  glm::vec3 new_left, new_right;

  float angleX = glm::orientedAngle(head_init, new_head, x_axis_ref);
  float angleY = glm::orientedAngle(head_init, new_head, y_axis_ref);
  float angleZ = glm::orientedAngle(head_init, new_head, z_axis_ref);

  new_left = glm::rotateX(left_init, angleX);
  new_left = glm::rotateY(left_init, angleY);
  new_left = glm::rotateZ(left_init, angleZ);
  new_right = glm::rotateX(right_init, angleX);
  new_right = glm::rotateY(right_init, angleY);
  new_right = glm::rotateZ(right_init, angleZ);

  A_BOID[0][0] = a_boid->pos[0];
  A_BOID[0][1] = a_boid->pos[1];
  A_BOID[0][2] = a_boid->pos[2];
  A_BOID[1][0] = new_head[0];
  A_BOID[1][1] = new_head[1];
  A_BOID[1][2] = new_head[2];
  A_BOID[2][0] = new_left[0];
  A_BOID[2][1] = new_left[1];
  A_BOID[2][2] = new_left[2];
  A_BOID[3][0] = new_right[0];
  A_BOID[3][1] = new_right[1];
  A_BOID[3][2] = new_right[2];
}
*/
