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

#define TOWER_HEIGHT              4000
#define CAMERA_NEAR               0.1
#define CAMERA_FAR                (TOWER_HEIGHT*200)

#define BG_SQUARE_SIDE            2000.0
#define BG_SQUARE_NUM             97   // must be an odd number

const GLfloat CLEAR_COLOR[3] = {0.182, 0.008, 0.235};				  
const GLfloat A_SQUARE[][3]= 
            {{0, 0, 0},
            {BG_SQUARE_SIDE, 0, 0},
            {BG_SQUARE_SIDE, BG_SQUARE_SIDE, 0},
            {0, BG_SQUARE_SIDE, 0}};

const float CHESS_BOARD_COLOUR_X[3] = {0.449,0.451,0.494};  //grey
const float CHESS_BOARD_COLOUR_Y[3] = {0.0, 0.0, 0.0};     //black

typedef enum viewMode {DEFAULT, TRAILING, SIDE} viewMode;

void change_view(viewMode viewmode, int width, int height, List *flock, 
                GOAL *goal);

void init_background(GLfloat squares_pos[][2]);
void draw_background(GLfloat squares_pos[][2]);
#endif
