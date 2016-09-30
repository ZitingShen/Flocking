#include "view.h"
#include "gl_replacement.h"

void change_view(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal) {
  glm::vec4 center = flock_centroid(flock);
  glm::vec4 midpoint = mid_point(flock, goal);
  float max_distance =  flock_radius(flock);
  float distance = get_d(flock, goal);
  glm::vec4 camera_pos;
  glm::vec4 flock_direction = glm::normalize(get_u(flock, goal));

  GLfloat eye[3];
  GLfloat centre[3];
  GLfloat up[3];
  switch(viewmode) {
    case DEFAULT:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(45, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    myPerspective(45, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0, 0.01, TOWER_HEIGHT, midpoint.x, midpoint.y, 
    //  midpoint.z, 0, 0, 1);
    eye[0] = 0;
    eye[1] = 0.01;
    eye[2] = TOWER_HEIGHT;
    centre[0] = midpoint[0];
    centre[1] = midpoint[1];
    centre[2] = midpoint[2];
    up[0] = 0;
    up[1] = 0;
    up[2] = 1;
    myLookAt(eye, centre, up);
    break;

    case TRAILING:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(30, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    myPerspective(30, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    camera_pos = center
                 - flock_direction*(distance + 2*max_distance)
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    //gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
    //          midpoint.z, 0, 0, 1);
    eye[0] = camera_pos[0];
    eye[1] = camera_pos[1];
    eye[2] = camera_pos[2];
    centre[0] = midpoint[0];
    centre[1] = midpoint[1];
    centre[2] = midpoint[2];
    up[0] = 0;
    up[1] = 0;
    up[2] = 1;
    myLookAt(eye, centre, up);
    break;

    case SIDE: {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //gluPerspective(40, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    myPerspective(40, width*1.0/height, CAMERA_NEAR, CAMERA_FAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec3 v3(flock_direction);
    glm::vec4 side_v = glm::vec4(glm::cross(v3, glm::vec3(0, 0, 1)), 0.0);
    camera_pos = midpoint
                 + glm::normalize(side_v)*(distance + 2*max_distance)
                 + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    //gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
    //          midpoint.z, 0, 0, 1);
    eye[0] = camera_pos[0];
    eye[1] = camera_pos[1];
    eye[2] = camera_pos[2];
    centre[0] = midpoint[0];
    centre[1] = midpoint[1];
    centre[2] = midpoint[2];
    up[0] = 0;
    up[1] = 0;
    up[2] = 1;
    myLookAt(eye, centre, up);
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
      glColor3f(CHESS_BOARD_COLOUR_X[0], CHESS_BOARD_COLOUR_X[1], CHESS_BOARD_COLOUR_X[2]);
    } else {
      glColor3f(CHESS_BOARD_COLOUR_Y[0], CHESS_BOARD_COLOUR_Y[1], CHESS_BOARD_COLOUR_Y[2]);
    }
    glPushMatrix();
    glTranslatef(squares_pos[i][0], squares_pos[i][1], -10);
    glPointSize(5);
    glDrawArrays(GL_QUADS, 0, 4);
    glPopMatrix();
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}
