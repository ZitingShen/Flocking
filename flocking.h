#ifndef FLOCKING_H
#define FLOCKING_H

#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif
#include <glm/glm.hpp>
#include "list.h"
#include "view.h"
#include "boid.h"

const glm::vec4 INITIAL_VELOCITY = glm::vec4(10.0,10.0,0,0);
const glm::vec4 INITIAL_POSITION = glm::vec4(2400.0,150.0,1200.0,1);
#define DEFAULT_SPAWN_CUBE_LENGTH 50.0
#define DEFAULT_FLOCKING_RADIUS   30.0
#define DEFAULT_FLOCK_SIZE        20
#define DEFAULT_SEPARATION_WEIGHT 0.3
#define DEFAULT_ALIGNMENT_WEIGHT  0.4
#define DEFAULT_COHESION_WEIGHT   0.5


#endif
