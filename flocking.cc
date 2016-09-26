#include "flocking.h"

void init();
void drawCube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);

List *flock;
GOAL goal;
int isPaused;
int paused_times;
viewMode viewmode;
int width, height;
GLfloat **boids_vertices;

int main(int argc, char** argv) {
  GLFWwindow* window;

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
  glfwSetKeyCallback(window, keyboard);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  init();
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);
  viewmode = DEFAULT;
  isPaused = GLFW_FALSE;
  paused_times = 0;
  goal.pos = zero_vec;
  flock = list_new();
  init_a_flock(flock, zero_vec, glm::vec4(0.0, 0.0, 0.0, 1.0), 
    DEFAULT_FLOCKING_RADIUS, DEFAULT_SPAWN_CUBE_LENGTH, DEFAULT_FLOCK_SIZE);
  boids_vertices = NULL;

  glfwGetWindowSize(window, &width, &height);
  changeView(viewmode, width, height, flock, &goal, DEFAULT_TOWER_HEIGHT);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwSwapBuffers(window);
    glfwPollEvents();

    if(!isPaused || paused_times > 0) {
      //moveBoidsVertices(flock, boids_vertices);
      //moveGoal(goal);
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        drawBackground();
        //drawFlock(boids_vertices);
        //drawFlock();
      }
      if (isPaused && paused_times > 0) {

        paused_times--;
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
  changeView(viewmode, w, h, flock, &goal, DEFAULT_TOWER_HEIGHT);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
      add_a_boid(flock);
      break;

      case GLFW_KEY_MINUS:
      remove_a_boid(flock);
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
      changeView(viewmode, width, height, flock, &goal, DEFAULT_TOWER_HEIGHT);
      break;

      case GLFW_KEY_T:
      viewmode = TRAILING;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height, flock, &goal, DEFAULT_TOWER_HEIGHT);
      break;

      case GLFW_KEY_G:
      viewmode = SIDE;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height, flock, &goal, DEFAULT_TOWER_HEIGHT);
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
