#include "flocking.h"

void init();
void draw_cube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void move_boids_pos(List* a_flock, GLfloat** boid_poly);
void draw_a_flock(List* a_flock);

List* A_FLOCK = NULL;

GOAL A_GOAL;
GLfloat A_BOID[VERTICES_PER_BOID][DIMENSIONS] = 
            {{0, 0, 0}, // position of the original centroid
            {0, 10, 0}, // position of the original head
            {-5, -5, 0}, // position of the original left vertex
            {5, -5, 0}}; // position of the original right vertex;
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
  changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL, TOWER_HEIGHT);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(!IS_PAUSED || PAUSE_TIME > 0) {
      //move_boids_pos(flock, boid_poly);
      //moveGoal(goal);
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        //drawBackground();
        draw_a_flock(A_FLOCK);
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
  changeView(VIEW_MODE, w, h, A_FLOCK, &A_GOAL, TOWER_HEIGHT);
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
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL, TOWER_HEIGHT);
      break;

      case GLFW_KEY_T:
      VIEW_MODE = TRAILING;
      glfwGetWindowSize(w, &WIDTH, &HEIGHT);
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL, TOWER_HEIGHT);
      break;

      case GLFW_KEY_G:
      VIEW_MODE = SIDE;
      glfwGetWindowSize(w, &WIDTH, &HEIGHT);
      changeView(VIEW_MODE, WIDTH, HEIGHT, A_FLOCK, &A_GOAL, TOWER_HEIGHT);
      break;

      case GLFW_KEY_A:
      break;

      case GLFW_KEY_D:
      break;

      case GLFW_KEY_W:
      break;

      case GLFW_KEY_S:
      break;

      case GLFW_KEY_Z:
      break;

      case GLFW_KEY_X:
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

void draw_a_flock(List* a_flock){
  if (a_flock == NULL) return;
  NODE* current = a_flock->head;
  BOID* some_boid = NULL;

  glEnableClientState(GL_VERTEX_ARRAY);
  glColor3f(0.0, 0.0, 0.0);
  glVertexPointer(3, GL_FLOAT, 0, A_BOID);

  for (int i = 0; i < a_flock->length; i++){
    some_boid = (BOID*)(current->data);
    
    glm::vec4 rotate_normal = glm::normalize(some_boid->velocity);
    GLfloat rotate_matrix[16] = {1, 0, 0, rotate_normal.x,
                                 0, 1, 0, rotate_normal.y,
                                 0, 0, 1, rotate_normal.z,
                                 0, 0, 0, 1};
    if (i == 0) {
      std::cout << some_boid->pos.x << " " << some_boid->pos.y << " " << some_boid->pos.z << std::endl;
      glm::vec4 midpoint = mid_point(A_FLOCK, &A_GOAL);
      std::cout << midpoint.x << " " << midpoint.y << " " << midpoint.z << std::endl;
    }
    glPushMatrix();
    glTranslatef(some_boid->pos.x, some_boid->pos.y, some_boid->pos.z);
    //glMultMatrixf(rotate_matrix);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, A_BOID_VERTICES);
    glPopMatrix();
    current = current->next;
  }
  glDisableClientState(GL_VERTEX_ARRAY);
}
