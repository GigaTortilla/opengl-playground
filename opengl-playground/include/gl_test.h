#ifndef GL_TEST_H
#define GL_TEST_H 1

#include <camera.h>
#include <glfw3.h>

#define X_AXIS (vec3s){ 1.0f, 0.0f, 0.0f }
#define Y_AXIS (vec3s){ 0.0f, 1.0f, 0.0f }
#define Z_AXIS (vec3s){ 0.0f, 0.0f, 1.0f }
#define NEAR_PLANE 0.1f
#define FAR_PLANE 100.0f

#define WINDOW_WIDTH 1200
#define WINDOW_HEIGHT 900
#define CENTER_OFFSET 0.2f

void render_image_mix();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initWindow(int width, int height);
unsigned int genBindVAO(unsigned int VBO, float vertices[], size_t size);
unsigned int genBindVBO(float vertices[], size_t size);
unsigned int genBindEBO(float vertices[], unsigned int indices[], size_t iSize);
void textureRectanglePointerArithmetic();
unsigned int genBindStdTexture(char* imgData, int width, int height);
void cam_updateMouse(Camera* cam);
void render_image_mix();
void spin_and_scale_2d();
void render_rectangle_3d();
void spin_cube();
void spin_10_cubes();
void rotate_scene();
void free_movement();
void xz_plane_movement();

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
static float cubeVertexCount = 36;

// cube positions
static float cubePositions[] = {
    0.0f,  0.0f,  0.0f,
    2.0f,  5.0f, -15.0f,
   -1.5f, -2.2f, -2.5f,
   -3.8f, -2.0f, -12.3f,
    2.4f, -0.4f, -3.5f,
   -1.7f,  3.0f, -7.5f,
    1.3f, -2.0f, -2.5f,
    1.5f,  2.0f, -2.5f,
    1.5f,  0.2f, -1.5f,
   -1.3f,  1.0f, -1.5f
};

// xplane
static float xzPlaneVertices[] = {
    // positions         // colors
   -100.0f, -0.5f, -100.0f,   0.2f, 0.4f, 0.1f,   // top right
   -100.0f, -0.5f,  100.0f,   0.2f, 0.4f, 0.1f,   // bottom right
    100.0f, -0.5f, -100.0f,   0.2f, 0.4f, 0.1f,   // bottom left
    100.0f, -0.5f,  100.0f,   0.2f, 0.4f, 0.1f,   // top left 
   -100.0f, -0.5f,  100.0f,   0.2f, 0.4f, 0.1f,   // bottom right
    100.0f, -0.5f, -100.0f,   0.2f, 0.4f, 0.1f,   // bottom left
};
#endif