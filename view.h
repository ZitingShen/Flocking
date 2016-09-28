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

#define TOWER_HEIGHT     		  100

typedef enum viewMode {DEFAULT, TRAILING, SIDE} viewMode;

void changeView(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal);
void initBackground();
void drawBackground();

void initBackground(int side, int square_num, GLfloat bg_vertices[][3], 
                    GLfloat bg_colors[][3]);
void drawBackground(int square_num, GLfloat bg_vertices[][3], 
                    GLfloat bg_colors[][3]);
#endif
