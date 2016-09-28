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

#define BOID_SIZE                 5

#define BG_SIDE_LENGTH			  20000
#define BG_SQUARE_NUM			  50
#endif
