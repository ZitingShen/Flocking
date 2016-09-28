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

const glm::vec4 EMPTY_POS   = glm::vec4(0.0,0.0,0.0,1.0);
const glm::vec4 SPAWN_VELOCITY = glm::vec4(0,10.0,0,0); //initial speed parallel with y-axis
#define DEFAULT_SPAWN_CUBE_LENGTH 50.0
#define DEFAULT_FLOCKING_RADIUS   30.0
#define DEFAULT_FLOCK_SIZE        20
#define DEFAULT_SEPARATION_WEIGHT 0.3
#define DEFAULT_ALIGNMENT_WEIGHT  0.4
#define DEFAULT_COHESION_WEIGHT   0.5
#define DEFAULT_ATTRACTION_WEIGHT 0.1
#define DEFAULT_DETERRENCE_WEIGHT 1.0 // this should be the most significant weight
#define DEFUALT_AVOIDANCE_WEIGHT  0.8 // this should be significant as well

#define DIMENSIONS                3
#define BOID_SIZE                 5

#define VERTICES_PER_BOID         4

#define TOWER_HEIGHT     		  100

#define BG_SIDE_LENGTH			  20000
#define BG_SQUARE_NUM			  50
#endif
