#include "view.h"

void changeView(viewMode viewmode, int width, int height, List *flock, 
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
    //gluLookAt(0, 0.01, 5, 0, 0, 0, 0, 0, 1);
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

void initBackground(int side, int square_num, GLfloat bg_vertices[][3], 
                    GLfloat bg_colors[][3]) {
  int index = 0;
  int square_side = side/square_num;
  for (int row = 0; row < square_num; row++) {
    for (int column = 0; column < square_num; column++) {
      bg_vertices[4*index][0] = column*square_side - side/2;
      bg_vertices[4*index][1] = row*square_side - side/2;
      bg_vertices[4*index][2] = 0;
      bg_vertices[4*index+1][0] = column*square_side - side/2;
      bg_vertices[4*index+1][1] = (row+1)*square_side - side/2;
      bg_vertices[4*index+1][2] = 0;
      bg_vertices[4*index+2][0] = (column+1)*square_side - side/2;
      bg_vertices[4*index+2][1] = (row+1)*square_side - side/2;
      bg_vertices[4*index+2][2] = 0;
      bg_vertices[4*index+3][0] = (column+1)*square_side - side/2; 
      bg_vertices[4*index+3][1] = row*square_side - side/2;
      bg_vertices[4*index+3][2] = 0;
      int color = (row+column)%2;
      bg_colors[index][0] = color;
      bg_colors[index][1] = color;
      bg_colors[index][2] = color;
      index++;
    }
  }

  GLuint vbos[2];
  glGenBuffers(2, vbos);
  glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
  glBufferData(GL_ARRAY_BUFFER, sizeof(bg_vertices), bg_vertices, 
    GL_STATIC_DRAW);

}

void drawBackground(int square_num, GLfloat bg_vertices[][3], 
                    GLfloat bg_colors[][3]) {
  glDrawArrays(GL_QUADS, 0, square_num*square_num);
}