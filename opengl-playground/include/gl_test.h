#ifndef GL_TEST_H
#define GL_TEST_H 1

#define X_AXIS (vec3s){ 1.0f, 0.0f, 0.0f }
#define Y_AXIS (vec3s){ 0.0f, 1.0f, 0.0f }
#define Z_AXIS (vec3s){ 0.0f, 0.0f, 1.0f }
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 900
#define CENTER_OFFSET 0.2f

void render_image_mix();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initWindow(int width, int height);
unsigned int genBindVAO(unsigned int VBO, float vertices[], size_t size);
unsigned int genBindVBO(float vertices[], size_t size);
unsigned int genBindEBO(float vertices[], unsigned int indices[], size_t iSize);
void textureRectanglePointerArithmetic();
void render_image_mix();
void spin_and_scale_2d();
void render_rectangle_3d();
void spin_cube();

// triangles
static float triVerticesOne[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
};
static float triVerticesTwo[] = {
	 0.9f, -0.5f, 0.0f,
	 0.1f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f
};

// triangle texture
static float texVertices[] = {
	 // positions         // colors           // texture coords
	 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
	-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};
static unsigned int texIndices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

// triangle with colored vertices
static float triVerticesColored[] = {
	// positions		// colors
	0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,
   -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,
	0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f
};

// two in one
static float twoTriVertices[] = {
	-0.9f, -0.5f, 0.0f,
	-0.1f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.9f, -0.5f, 0.0f,
	 0.1f, -0.5f, 0.0f,
	 0.5f,  0.5f, 0.0f
};

// rectangle
static float recVertices[] = {
	 0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f
};
static unsigned int recIndices[] = {
	0, 1, 3,
	1, 2, 3
};

// cube vertices
static float cubeVertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
#endif