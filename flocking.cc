#include "flocking.h"

void init();
void draw_cube();
void reshape(GLFWwindow* window, int w, int h);
void framebuffer_resize(GLFWwindow* window, int width, int height);
void keyboard(GLFWwindow *w, int key, int scancode, int action, int mods);
void move_boids_pos(List* a_flock, GLfloat** boid_poly);
//void generate_poly_vertices(List* a_flock, GLfloat** boid_poly, GOAL* a_goal);
void draw_a_flock(List* a_flock);

List* A_FLOCK = NULL;
GLfloat** BOID_POLY = NULL;

GOAL A_GOAL;
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

  A_GOAL.pos = zero_vec;
  initBackground(BG_SIDE_LENGTH, BG_SQUARE_NUM, bg_vertices, bg_colors);

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
        drawBackground();
      //draw_boids(boid_poly, vertices_per_boid);
        drawBackground(BG_SQUARE_NUM, bg_vertices, bg_colors);
        //drawFlock(boids_vertices);
        //drawFlock();
        //drawCube();
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
  init_a_flock(A_FLOCK, SPAWN_VELOCITY, DEFAULT_FLOCKING_RADIUS,
               DEFAULT_SPAWN_CUBE_LENGTH, DEFAULT_FLOCK_SIZE);
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

void generate_poly_vertices(List* a_flock, GLfloat** boid_poly, GOAL* a_goal){
  int flock_size = a_flock->length;
  NODE* current = a_flock->head;
  BOID* a_boid  = NULL;
  int vertex_count = 0;
  glm::vec3 boid_pos, goal_pos, velocity, head, left, right, normal_to_GAP, normal_to_ABC;
  if (boid_poly!=NULL){
    for (int i=0; i<flock_size; i++){
      delete boid_poly[i];
    }
    delete [] boid_poly;
  }
  /* calculating vertices to draw a boid */
  boid_poly = new GLfloat*[flock_size * VERTICES_PER_BOID];
  for (int i=0; i<(flock_size); i++){
    a_boid = (BOID*)current->data;
    for (int j=0;j<DIMENSIONS;j++){ // convert to vec3 for calculation
      boid_pos[j] = a_boid->pos[j];
      goal_pos[j] = a_goal->pos[j];
      velocity[j] = a_boid->velocity[j];
    }
    head = boid_pos + static_cast<float>(BOID_SIZE)*normalize(velocity);
    normal_to_GAP = glm::cross(goal_pos-head, goal_pos-boid_pos);
    normal_to_ABC = boid_pos + glm::rotate(head-boid_pos,(GLfloat)(90.0), normal_to_GAP);
    left = boid_pos + glm::rotate(head-boid_pos, (GLfloat)(120.0)*1.0f, normal_to_ABC);
    right= boid_pos + glm::rotate(head-boid_pos, (GLfloat)(-120.0), normal_to_ABC);
    while (vertex_count < VERTICES_PER_BOID){
      boid_poly[i*VERTICES_PER_BOID+vertex_count]= new GLfloat[DIMENSIONS];
      switch(vertex_count){
        case 0: //first vertex: centorid
          boid_poly[i*VERTICES_PER_BOID+vertex_count][0] = boid_pos[0];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][1] = boid_pos[1];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][2] = boid_pos[2];
          break;
        case 1: //second vertex: head
          boid_poly[i*VERTICES_PER_BOID+vertex_count][0] = head[0];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][1] = head[1];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][2] = head[2];
          break;
        case 2: //third vertex: left
          boid_poly[i*VERTICES_PER_BOID+vertex_count][0] = left[0];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][1] = left[1];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][2] = left[2];
          break;
        case 3: //third vertex: right
          boid_poly[i*VERTICES_PER_BOID+vertex_count][0] = right[0];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][1] = right[1];
          boid_poly[i*VERTICES_PER_BOID+vertex_count][2] = right[2];
          break;
        default:
          break;
      }
      vertex_count++;
    }
    current=current->next;
    vertex_count = 0;
  }
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
}

void draw_a_flock(List* a_flock){
  if (a_flock == NULL) return;
  float speed = glm::length(SPAWN_VELOCITY);
  glm::vec4 centroid = SPAWN_POSITION;
  glm::vec4 head = centroid +
                   (static_cast<float>(BOID_SIZE))*glm::normalize(SPAWN_VELOCITY);
  glm::vec4 left = glm::vec4(-speed, -0.5*speed,0,1);
  glm::vec4 right = glm::vec4(speed, -0.5*speed,0,1);
  glm::vec4 new_centroid, new_head, new_left, new_right;
  GLfloat vertices[VERTICES_PER_BOID][DIMENSIONS]; // we have four vertices in R3
  GLfloat colors[3] = {1.0,1.0,1.0}; //using black
  GLubyte triangleIndices[6] = {0,1,2,
                                0,1,3}; //drawing two triangles
  glm::vec3 current_pos, sourcePA, targetPA;
  NODE* current = a_flock->head;
  glm::fquat rotation_angle;
  glm::mat4 rotation_maxtrix;
  BOID* a_boid = NULL;

  for (int i=0;i<a_flock->length;i++){
    a_boid = (BOID*)(current->data);
    //new_centroid = get_current_pos(a_boid); //first vertex
    sourcePA = glm::vec3(SPAWN_VELOCITY[0], SPAWN_VELOCITY[1], SPAWN_VELOCITY[2]);
    targetPA = glm::vec3(a_boid->velocity[0], a_boid->velocity[2], a_boid->velocity[2]);
    rotation_angle = glm::rotation(
                        glm::normalize(sourcePA),
                        glm::normalize(targetPA));
    for (int j=0;j<VERTICES_PER_BOID;j++){
    
    }
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);

    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_VERTEX_ARRAY);
    current = current->next;
  }
}
