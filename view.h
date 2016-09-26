#ifndef VIEW_H
#define VIEW_H

#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#include <GL/glu.h>
#endif
#include <glm/glm.hpp>
#include "boid.h"

typedef enum viewMode {DEFAULT, TRAILING, SIDE} viewMode;

void changeView(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal, int tower_h);
void drawBackground();
#endif
