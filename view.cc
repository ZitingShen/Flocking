#include "view.h"
#include <iostream>

void change_view(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal) {
  glm::vec4 center = flock_centroid(flock);
  glm::vec4 midpoint = mid_point(flock, goal);
  float max_distance =  flock_radius(flock);
  float distance = get_d(flock, goal);
  glm::vec4 camera_pos;
  glm::vec4 flock_direction = get_u(flock, goal);
  switch(viewmode) {
    case DEFAULT:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0.01, TOWER_HEIGHT, midpoint.x, midpoint.y, midpoint.z, 
              0, 0, 1);
    //std::cout << midpoint.x << " " << midpoint.y << " " << midpoint.z << std::endl;
    break;

    case TRAILING:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera_pos = center
                 + glm::normalize(-flock_direction)*(distance + 5*max_distance)
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 1, 0);
    break;

    case SIDE: {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec3 v(flock_direction);
    glm::vec4 side_v = glm::vec4(glm::cross(v, glm::vec3(0, 0, 1)), 0.0);
    camera_pos = midpoint 
                 + glm::normalize(side_v)*(distance + 2*max_distance);
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 1, 0);
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
  glVertexPointer(4, GL_FLOAT, 0, A_SQUARE);
  for (int i = 0; i < BG_SQUARE_NUM*BG_SQUARE_NUM; i++) {
    if(i % 2 == 0) {
      glColor3f(1.0, 0.0, 0.0);
    } else {
      glColor3f(0.0, 0.0, 1.0);
    }
    glPushMatrix();
      
    glTranslatef(squares_pos[i][0], squares_pos[i][1], 10);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}