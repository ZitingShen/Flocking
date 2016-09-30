#ifndef FLOCKING_H
#define FLOCKING_H

#include <iostream>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp> // for rotate
#include <glm/gtx/quaternion.hpp>    // for rotation angle
#include "list.h"
#include "view.h"
#include "boid.h"
#include "flocking.h"
//#include "obstacle.h"

void init();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
#endif
