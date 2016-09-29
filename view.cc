#include "view.h"
#include <iostream>

void change_view(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal) {
  glm::vec4 center = flock_centroid(flock);
  glm::vec4 midpoint = mid_point(flock, goal);
  float max_distance =  flock_radius(flock);
  float distance = get_d(flock, goal);
  glm::vec4 camera_pos;
  glm::vec4 flock_direction = glm::normalize(get_u(flock, goal));
  switch(viewmode) {
    case DEFAULT:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0.01, TOWER_HEIGHT, midpoint.x, midpoint.y, 
      midpoint.z, 0, 0, 1);
    break;

    case TRAILING:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera_pos = center
                 - flock_direction*(distance + 5*max_distance)*((float)0.5)
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 0, 1);
    break;

    case SIDE: {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec3 v3(flock_direction);
    glm::vec4 side_v = glm::vec4(glm::cross(v3, glm::vec3(0, 0, 1)), 0.0);
    camera_pos = midpoint 
                 + glm::normalize(side_v)*(distance + 2*max_distance)
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 0, 1);
    }
    break;
    default:
    break;
  }
}

void init_background(GLfloat squares_pos[][2]) {
  int index = 0;
  for (int row = 0; row < BG_SQUARE_NUM; row++) {
    for (int column = 0; column < BG_SQUARE_NUM; column++) {
      squares_pos[index][0] = (row - BG_SQUARE_NUM/2.0)*BG_SQUARE_SIDE;
      squares_pos[index][1] = (column - BG_SQUARE_NUM/2.0)*BG_SQUARE_SIDE;
      index++;
    }
  }
}

void draw_background(GLfloat squares_pos[][2]) {
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, A_SQUARE);
  for (int i = 0; i < BG_SQUARE_NUM*BG_SQUARE_NUM; i++) {
    if(i % 2 == 0) {
      glColor3f(1.0, 0.6, 0.6);
    } else {
      glColor3f(0.6, 0.6, 1.0);
    }
    glPushMatrix();
    glTranslatef(squares_pos[i][0], squares_pos[i][1], -10);
    glPointSize(5);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}