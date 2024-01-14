#ifndef GL_TEST_H
#define GL_TEST_H 1
void glTest();
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
GLFWwindow* initWindow(int width, int height);

static int WINDOW_WIDTH = 1600;
static int WINDOW_HEIGHT = 900;

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
#endif