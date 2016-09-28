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

#define TOWER_HEIGHT              100

#define BG_SQUARE_SIDE            10.0
#define BG_SQUARE_NUM             50
const GLfloat A_SQUARE[][3] = 
            {{0, 0, 0},
            {BG_SQUARE_SIDE, 0, 0},
            {BG_SQUARE_SIDE, BG_SQUARE_SIDE, 0},
            {0, BG_SQUARE_SIDE, 0}};
const GLubyte A_SQUARE_VERTICES[6] = {0, 1, 2,
                                      0, 2, 3};

typedef enum viewMode {DEFAULT, TRAILING, SIDE} viewMode;

void change_view(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal);

void init_background(GLfloat squares_pos[][2]);
void draw_background(GLfloat squares_pos[][2]);
#endif
