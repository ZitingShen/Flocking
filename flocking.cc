#include "flocking.h"

void init();
void draw_cube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void move_boids_pos(List* a_flock, GLfloat** boid_poly, int vertices_per_boid);
void draw_boids(GLfloat** boid_poly, int vertices_per_boid);

List* flock;

GOAL goal;
int isPaused;
int paused_times;
viewMode viewmode;
int width, height;
GLfloat **boids_poly;
GLfloat **boids_vertices;
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

  viewmode = DEFAULT;
  isPaused = GLFW_FALSE;
  paused_times = 0;
  goal.pos = zero_vec;
  flock = list_new();
  init_a_flock(flock, zero_vec, glm::vec4(0.0, 0.0, 0.0, 1.0), 
    DEFAULT_FLOCKING_RADIUS, DEFAULT_SPAWN_CUBE_LENGTH, DEFAULT_FLOCK_SIZE);
  boids_poly = NULL;
  initBackground(BG_SIDE_LENGTH, BG_SQUARE_NUM, bg_vertices, bg_colors);
  boids_vertices = NULL;

  init();
  
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glShadeModel(GL_FLAT);

  glfwGetWindowSize(window, &width, &height);
  changeView(viewmode, width, height, flock, &goal, TOWER_HEIGHT);

  while(!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glfwPollEvents();

    if(!isPaused || paused_times > 0) {
      //move_boids_pos(flock, boid_poly);
      //moveGoal(goal);
      if(glfwGetWindowAttrib(window, GLFW_VISIBLE)){
        drawBackground();
      //draw_boids(boid_poly, vertices_per_boid);
        drawBackground(BG_SQUARE_NUM, bg_vertices, bg_colors);
        //drawFlock(boids_vertices);
        //drawFlock();
        //drawCube();
      }
      glfwSwapBuffers(window);
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
  changeView(viewmode, w, h, flock, &goal, TOWER_HEIGHT);
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
      changeView(viewmode, width, height, flock, &goal, TOWER_HEIGHT);
      break;

      case GLFW_KEY_T:
      viewmode = TRAILING;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height, flock, &goal, TOWER_HEIGHT);
      break;

      case GLFW_KEY_G:
      viewmode = SIDE;
      glfwGetWindowSize(w, &width, &height);
      changeView(viewmode, width, height, flock, &goal, TOWER_HEIGHT);
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

void generate_poly_vertices(GLfloat** boid_pos, GLfloat** boid_poly,
                            int vertices_per_boid, int flock_size){
  if (boid_poly!=NULL){
    for (int i=0; i<flock_size; i++){
      delete boid_poly[i];
    }
    delete [] boid_poly;
  }

  boid_poly = new GLfloat*[flock_size];
  for (int i=0; i<flock_size; i++){
    boid_poly[i]= new GLfloat[vertices_per_boid];
  }
  /* drawing two triangles */

}


void move_boids_pos(List* a_flock, GLfloat** boid_poly, int vertices_per_boid){
  int flock_size = a_flock->length;
  NODE* current = a_flock->head;

  GLfloat** boid_pos = new GLfloat*[flock_size];
  for (int i=0; i<flock_size; i++){
    boid_pos[i] = new GLfloat[DIMENSIONS];
  }

  for (int i=0; i<flock_size; i++){
    boid_pos[i][0] = ((BOID*)(current->data))->pos[0];
    boid_pos[i][1] = ((BOID*)(current->data))->pos[1];
    boid_pos[i][2] = ((BOID*)(current->data))->pos[2];
    boid_pos[i][3] = 1.0; // position is a point
  }
  generate_poly_vertices(boid_pos, boid_poly, vertices_per_boid, flock_size);
}

void draw_cube() {
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
