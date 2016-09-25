#include "view.h"

void changeView(viewMode viewmode, int width, int height, List *flock, 
                GOAL ＊goal) {
  glm::vec4 center ＝ flock_centroid(List* a_flock);
  glm::vec4 midpoint = mid_point(List* a_flock, GOAL* a_goal);
  float max_distance ＝ flock_radius(List* a_flock);
  float distance = get_d(List* a_flock, GOAL* a_goal);
  switch(viewmode) {
    case DEFAULT:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, DEFAULT_VIEW_Y, 1, midpoint.x, midpoint.y, midpoint.z, 
              0, 1, 0);
    break;

    case TRAILING:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(30, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec4 camera_pos = center 
                      + glm::normalize(center - goal)*(distance + 5*max_distance)
                      + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 1, 0);
    break;

    case SIDE:
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, width*1.0/height, 0.1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glm::vec4 side_v = glm::vec4(glm::cross((center - goal).xyz, 
                           glm::vec3(0, 0, 1)), 0.0);
    glm::vec4 camera_pos = midpoint 
                          + glm::normalize(side_v)*(distance + 2*max_distance);
                          + glm::vec4(0, 0, 1, 0)*(distance + max_distance);
    gluLookAt(camera_pos.x, camera_pos.y, camera_pos.z, midpoint.x, midpoint.y, 
              midpoint.z, 0, 1, 0);
    break;
    default:
    break;
  }
}