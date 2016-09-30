#include "gl_replacement.h"

void myLookAt(GLfloat eye[3], GLfloat centre[3], GLfloat up[3]){
  glm::vec3 forward_normal =
     glm::normalize(glm::vec3(centre[0] - eye[0],
                              centre[1] - eye[1],
                              centre[2] - eye[2]));
  glm::vec3 side_normal =
    glm::normalize(glm::cross(forward_normal, glm::vec3(up[0],up[1],up[2])));

  glm::vec3 up_normal = glm::cross(side_normal, forward_normal);

  GLfloat trans_matrix[16] = //putting in column-major order
             {side_normal[0], up_normal[0], -forward_normal[0], 0,
              side_normal[1], up_normal[1], -forward_normal[1], 0,
              side_normal[2], up_normal[2], -forward_normal[2], 0,
              0,0,0,1};

  glMultMatrixf(trans_matrix);
  glTranslatef(-eye[0], -eye[1], -eye[2]); //move to eye
}

void myPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar){
  GLfloat y_max = tan(fovy * DEGREE_TO_RADIAN) * zNear;
  GLfloat y_min = -y_max;
  GLfloat x_max = y_max * aspect;
  GLfloat x_min = -x_max;

  GLfloat trans_matrix[16] = //putting in column-major order
       {2*zNear*aspect/(x_max-x_min), 0, 0, 0,
        0, 2*zNear/(y_max-y_min), 0, 0,
        0, 0, -(zFar+zNear)/(zFar-zNear), -1,
        0, 0, -2*(zFar*zNear)/(zFar-zNear),0};

  glMultMatrixf(trans_matrix);
}
