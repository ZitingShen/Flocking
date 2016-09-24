#include <stdio.h>
#include <stdlib.h>
#ifdef __APPLE__
#include <GLFW/glfw3.h>
#include <OpenGL/glu.h>
#else
#include <GLFW/glfw3.h>
#endif
#include "list.h"

void init();
void drawCube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);

list boids;

int main(int argc, char** argv) {
  GLFWwindow* window;

  // Initialize the library
  if (!glfwInit())
    exit(EXIT_FAILURE);

  // Create a window and its OpenGL context
  window = glfwCreateWindow(500, 500, "Cube", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, 100, 0);

  // Make the window's context current
  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  angle = 0;

  init();

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, 1, 0.1, 1000);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0, 3, 5, 0, 0, 0, 0, 1, 0);
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_SMOOTH);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glRotatef(angle, 0, 1, 0);

    if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
      drawFlock();
    }
    glPopMatrix();
    angle += 0.5;
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init() {
  // set the background color to black
  glClearColor(0.0, 0.0, 0.0, 1.0);
  // Set the drawing color to white
  glColor3f(1.0, 1.0, 1.0);
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
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