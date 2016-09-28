#include "flocking.h"

void init();
void draw_cube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void move_boids_pos(List* a_flock, GLfloat** boid_poly);

List* A_FLOCK = NULL;

GOAL A_GOAL;
GLfloat A_BOID[VERTICES_PER_BOID][DIMENSIONS] = 
            {{0, 0, 0}, // position of the original centroid
            {0, BOID_SIZE*2, 0}, // position of the original head
            {-BOID_SIZE, -BOID_SIZE, 0}, // position of the original left vertex
            {BOID_SIZE, -BOID_SIZE, 0}}; // position of the original right vertex;
GLubyte A_BOID_VERTICES[6] = {0, 1, 2,
                              0, 3, 1}; //drawing two triangles;
int IS_PAUSED = GLFW_FALSE;
int PAUSE_TIME = 0;
viewMode VIEW_MODE = DEFAULT;
int WIDTH, HEIGHT;
GLfloat bg_vertices[BG_SQUARE_NUM*BG_SQUARE_NUM*4][3];
GLfloat bg_colors[BG_SQUARE_NUM*BG_SQUARE_NUM][3];

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

  
  //initBackground(BG_SIDE_LENGTH, BG_SQUARE_NUM, bg_vertices, bg_colors);

  init();

  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);

  glfwGetWindowSize(window, &WIDTH, &HEIGHT);
  changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      update_velocity(A_FLOCK);
      update_pos(A_FLOCK);
      //moveGoal(goal);
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        //drawBackground();
        draw_a_flock(A_FLOCK, A_BOID, A_BOID_VERTICES);
        //drawBackground(BG_SQUARE_NUM, bg_vertices, bg_colors);
        //drawFlock(boids_vertices);
        //drawFlock();
        //draw_cube();
      }
      glfwSwapBuffers(window);
      if (IS_PAUSED && PAUSE_TIME > 0) {
        PAUSE_TIME--;
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
  // initialise a flock of boid
  A_FLOCK = list_new();
  A_GOAL.pos = zero_vec;
  srand(time(NULL));
  init_a_flock(A_FLOCK);
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  changeView(VIEW_MODE, w, h, A_FLOCK, &A_GOAL);
}

void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods) {
  if (action == GLFW_PRESS) {
    switch(key) {
      case GLFW_KEY_EQUAL:
      add_a_boid(A_FLOCK);
      break;

      case GLFW_KEY_MINUS:
      remove_a_boid(A_FLOCK);
      break;

      case GLFW_KEY_P:
      IS_PAUSED = GLFW_TRUE;
      PAUSE_TIME++;
      break;

      case GLFW_KEY_R:
      IS_PAUSED = GLFW_FALSE;
      PAUSE_TIME = 0;
      break;

      case GLFW_KEY_V:
      VIEW_MODE  = DEFAULT;
      glfwGetWindowSize(w, &WIDTH, &HEIGHT);
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL);
      break;

      case GLFW_KEY_T:
      VIEW_MODE = TRAILING;
      glfwGetWindowSize(w, &WIDTH, &HEIGHT);
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL);
      break;

      case GLFW_KEY_G:
      VIEW_MODE = SIDE;
      glfwGetWindowSize(w, &WIDTH, &HEIGHT);
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL);
      break;

      case GLFW_KEY_A: // decrease x velocity
      break;

      case GLFW_KEY_D: // increase x velocity
      break;

      case GLFW_KEY_W: // increase y velocity
      break;

      case GLFW_KEY_S: // decrease y velocity
      break;

      case GLFW_KEY_Z: // increase z velocity
      break;

      case GLFW_KEY_X: // decrease z velocity
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
