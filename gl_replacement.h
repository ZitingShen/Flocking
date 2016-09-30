#ifndef GL_REPLACEMENT_H
#define GL_REPLACEMENT_H

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif

#include <glm/glm.hpp>

#define	DEGREE_TO_RADIAN				0.00873

void myLookAt(GLfloat eye[3], GLfloat center[3], GLfloat up[3]);
void myPerspective(GLfloat fovy, GLfloat aspect, GLfloat zNear, GLfloat zFar);

#endif
