#include "flocking.h"

List* A_FLOCK = NULL;
GOAL* A_GOAL = NULL;
//OBSTACLE* AN_OBSTACLE = NULL;

int IS_PAUSED = GLFW_FALSE;
int PAUSE_TIME = 0;
viewMode VIEW_MODE = DEFAULT;
int WIDTH, HEIGHT;
GLfloat SQUARES_POS[BG_SQUARE_NUM*BG_SQUARE_NUM][2];

int main(int argc, char** argv) {
  GLFWwindow* window;

  if (!glfwInit())
    exit(EXIT_FAILURE);

  window = glfwCreateWindow(500, 500, "Flocking", NULL, NULL);
  if (!window){
    glfwTerminate();
    exit(EXIT_FAILURE);
  }
  glfwSetWindowPos(window, 100, 0);

  glfwMakeContextCurrent(window);
  glfwSetWindowSizeCallback(window, reshape);
  glfwSetKeyCallback(window, keyboard);
  glfwSetFramebufferSizeCallback(window, framebuffer_resize);

  init();

  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      glfwGetWindowSize(window, &WIDTH, &HEIGHT);
      change_view(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, A_GOAL);
      update_goal_velocity(A_GOAL);
      update_goal_pos(A_GOAL);
      update_velocity(A_FLOCK);
      update_wing_rotation(A_FLOCK);
      apply_goal_attraction(A_FLOCK, A_GOAL);
      //if (AN_OBSTACLE->enable){
      //  apply_obstacle_avoidance(A_FLOCK, AN_OBSTACLE);
      //  draw_an_obstacle(AN_OBSTACLE);
      //}
      update_pos(A_FLOCK);
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        draw_background(SQUARES_POS);
        draw_a_flock(A_FLOCK);
        draw_a_goal(A_GOAL);
      }
      glfwSwapBuffers(window);
      if (IS_PAUSED && PAUSE_TIME > 0) {
        print_goal(A_GOAL);
        print_flock(A_FLOCK);
        PAUSE_TIME--;
      }
    }
  }
  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void init() {
  glClearColor(CLEAR_COLOR[0], CLEAR_COLOR[1], CLEAR_COLOR[2], 1.0);
  glColor3f(0.0, 0.0, 0.0);
  A_FLOCK = list_new();
  A_GOAL = new_goal();
  //AN_OBSTACLE = new_obstacle();
  srand(time(NULL));
  init_a_flock(A_FLOCK);
  init_background(SQUARES_POS);
}

void framebuffer_resize(GLFWwindow* window, int width, int height) {
   glViewport(0, 0, width, height);
}

void reshape(GLFWwindow* window, int w, int h) {
  change_view(VIEW_MODE, w, h, A_FLOCK, A_GOAL);
}

void cleanup(){
  free(A_GOAL);
  //free(AN_OBSTACLE);
  list_destroy(A_FLOCK);
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
      break;

      case GLFW_KEY_T:
      VIEW_MODE = TRAILING;
      break;

      case GLFW_KEY_G:
      VIEW_MODE = SIDE;
      break;

      case GLFW_KEY_A:
      A_GOAL->MOVE_ALONG_X_NEGATIVE = true;
      break;

      case GLFW_KEY_D:
      A_GOAL->MOVE_ALONG_X_POSITIVE = true;
      break;

      case GLFW_KEY_W:
      A_GOAL->MOVE_ALONG_Y_POSITIVE = true;
      break;

      case GLFW_KEY_S:
      A_GOAL->MOVE_ALONG_Y_NEGATIVE = true;
      break;

      case GLFW_KEY_RIGHT:
      A_GOAL->ACCELERATE = true;
      break;

      case GLFW_KEY_LEFT:
      A_GOAL->DECELERATE = true;
      break;

      //case GLFW_KEY_B:
      //AN_OBSTACLE->enable = !AN_OBSTACLE->enable;
      //break;

      case GLFW_KEY_Q:
      case GLFW_KEY_ESCAPE:
      glfwSetWindowShouldClose(w, GLFW_TRUE);
      break;
      default:
      break;
    }
  } else if (action == GLFW_RELEASE) {
    switch(key) {
      case GLFW_KEY_A:
      A_GOAL->MOVE_ALONG_X_NEGATIVE = false;
      break;

      case GLFW_KEY_D:
      A_GOAL->MOVE_ALONG_X_POSITIVE = false;
      break;

      case GLFW_KEY_W:
      A_GOAL->MOVE_ALONG_Y_POSITIVE = false;
      break;

      case GLFW_KEY_S:
      A_GOAL->MOVE_ALONG_Y_NEGATIVE = false;

      case GLFW_KEY_RIGHT:
      A_GOAL->ACCELERATE = false;
      break;

      case GLFW_KEY_LEFT:
      A_GOAL->DECELERATE = false;
      break;

      default:
      break;
    }
  }
}
