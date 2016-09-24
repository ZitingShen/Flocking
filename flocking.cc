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

void init();
void drawCube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);

flock boids;
int isPaused;
int paused_times;
viewMode viewmode;

int main(int argc, char** argv) {
  GLFWwindow* window;
  int width, height;

  // Initialize the library
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Create a window and its OpenGL context
  window = glfwCreateWindow(500, 500, "Flocking", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, 100, 0);

  // Make the window's context current
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  init();
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  viewmode = DEFAULT;

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(!isPaused || paused_times > 0) {
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        drawFlock();
      }
      glfwSwapBuffers(window);
      if (isPaused && paused_times > 0) {

      }
    }
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init() {
  // set the background color to white
  glClearColor(1.0, 1.0, 1.0, 1.0);
  // Set the drawing color to black
  glColor3f(0.0, 0.0, 0.0);
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  changeView(viewmode, w, h);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
      boids.add();
      break;

      case GLFW_KEY_MINUS:
      boids.delete();
      break;

      case GLFW_KEY_P:
      isPaused = GLFW_TRUE;
      paused_times++;
      break;

      case GLFW_KEY_R:
      isPaused = GLFW_FALSE;
      paused_times = 0;
      break;

      case GLFW_KEY_V:
      viewmode = DEFAULT;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height);
      break;

      case GLFW_KEY_T:
      viewmode = TRAILING;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height);
      break;

      case GLFW_KEY_G:
      viewmode = SIDE;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height);
      break;

      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(w, GLFW_TRUE);
      break;
      default:
      break;
    }
  } else if (action == GLFW_RELEASE) {
    switch(key) {
      default:
      break;
    }
  }
}




void drawCube() {
  GLfloat vertices[][3] = {{-1.0, -1.0, 1.0}, {-1.0, 1.0, 1.0}, 
                           {1.0, 1.0, 1.0}, {1.0, -1.0, 1.0}, 
                           {-1.0, -1.0, -1.0}, {-1.0, 1.0, -1.0}, 
                           {1.0, 1.0, -1.0}, {1.0, -1.0, -1.0}};
  GLfloat colors[][3] = {{0.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, 
                         {1.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, 
                         {0.0, 0.0, 1.0}, {1.0, 0.0, 1.0}, 
                         {1.0, 1.0, 1.0}, {0.0, 1.0, 1.0}};
  GLubyte cubeIndices[24] = {0, 3, 2, 1,
                             2, 3, 7, 6,
                             0, 4, 7, 3,
                             1, 2, 6, 5,
                             4, 5, 6, 7,
                             0, 1, 5, 4};
  glEnableClientState(GL_COLOR_ARRAY);
  glEnableClientState(GL_VERTEX_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vertices);
  glColorPointer(3, GL_FLOAT, 0, colors);
  glDrawElements(GL_QUADS, 24, GL_UNSIGNED_BYTE, cubeIndices);
  glDisableClientState(GL_COLOR_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}