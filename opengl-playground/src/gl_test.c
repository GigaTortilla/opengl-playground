#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <gl_test.h>
#include <shaders.h>
#include <unistd.h>
#include <string.h>
#include <stb_image.h>
#include <Windows.h>
#include <cglm/struct.h>

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
GLFWwindow* initWindow(int width, int height) {
	// glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE, 3);

	// create window object
	GLFWwindow* window = glfwCreateWindow(width, height, "GL Test", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}
	return window;
}
unsigned int genBindVAO(unsigned int VBO, float vertices[], size_t size) {
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	return VAO;
}
unsigned int genBindVBO(float vertices[], size_t size) {
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	return VBO;
}
unsigned int genBindEBO(float vertices[], unsigned int indices[], size_t iSize) {
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, iSize, indices, GL_STATIC_DRAW);

	return EBO;
}
void textureRectanglePointerArithmetic() {
	// pointer arithmetic for attributes
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
unsigned int genBindStdTexture(byte* imgData, int width, int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create texture and free data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(imgData);
	return texture;
}
// render 2 textures into a rectangle and continuously mix them together
void render_image_mix() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, texVertices, sizeof(texVertices));

	// rectangle EBO
	unsigned int EBO = genBindEBO(texVertices, texIndices, sizeof(texIndices));

	// build and use
	unsigned int program = buildShaderProgram("render_image_mix.vert", "render_image_mix.frag");
	glUseProgram(program);

	textureRectanglePointerArithmetic();
	
	// load images
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* imgData1 = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	unsigned char* imgData2 = stbi_load("images\\eternaforest.jpg", &width, &height, &nrChannels, 0);
	if (!imgData1 || !imgData2) {
		printf("Could not read image file.");
		return;
	}

	// texture generation
	unsigned int textures[2];
	textures[0] = genBindStdTexture(imgData1, width, height);
	textures[1] = genBindStdTexture(imgData2, width, height);

	glUniform1i(glGetUniformLocation(program, "texture1"), 0);
	glUniform1i(glGetUniformLocation(program, "texture2"), 1);

	unsigned int mixture = glGetUniformLocation(program, "mixture");

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// reload color
		glClearColor(0.2f, 0.3f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		double time = glfwGetTime();
		double mix = (sin(time) + 1.0f) / 2.0f;

		glUniform1f(mixture, mix);

		// bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[1]);

		// draw
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		// scuffed frame limiter
		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void spin_and_scale_2d() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, texVertices, sizeof(texVertices));

	// rectangle EBO
	unsigned int EBO = genBindEBO(texVertices, texIndices, sizeof(texIndices));

	// build and use
	unsigned int program = buildShaderProgram("rotate1.vert", "rotate1.frag");
	glUseProgram(program);

	textureRectanglePointerArithmetic();

	// load images
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	unsigned char* imgData1 = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	unsigned char* imgData2 = stbi_load("images\\eternaforest.jpg", &width, &height, &nrChannels, 0);
	if (!imgData1 || !imgData2) {
		printf("Could not read image file.");
		return;
	}

	// texture generation
	unsigned int textures[2];
	textures[0] = genBindStdTexture(imgData1, width, height);
	textures[1] = genBindStdTexture(imgData2, width, height);
	
	vec3s trans_vec1 = {
		.x = 0.5f, .y = 0.5f
	};
	vec3s trans_vec2 = {
		.x = -0.5f, .y = -0.5f
	};

	unsigned int transformLoc = glGetUniformLocation(program, "transform");

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// reload color
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// first texture
		double time = 0.0 - glfwGetTime();
		mat4s mat1 = glms_translate_make(trans_vec1);
		mat1 = glms_rotate(mat1, time, Z_AXIS);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, mat1.raw);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[0]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		double sin_time = sin(time) / 2 + 0.5;
		vec3s scal_vec = {
			.x = sin_time, .y = sin_time, .z = sin_time
		};

		// second texture
		mat4s mat2 = glms_translate_make(trans_vec2);
		mat2 = glms_scale(mat2, scal_vec);
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, mat2.raw);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// scuffed frame limiter
		Sleep(1);
	}
}
void render_rectangle_3d() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// matrices
	mat4s ortho = glms_ortho(0.0f, (float)WINDOW_WIDTH, 0.0f, (float)WINDOW_HEIGHT, 0.1f, 100.0f);
	mat4s perspective = glms_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	mat4s model = glms_rotate_make(glm_rad(-55.0f), X_AXIS);
	mat4s view = glms_translate_make((vec3s){ 0.0f, 0.0f, -3.0f });

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	genBindVAO(VBO, texVertices, sizeof(texVertices));

	// rectangle EBO
	unsigned int EBO = genBindEBO(texVertices, texIndices, sizeof(texIndices));

	textureRectanglePointerArithmetic();

	// shader program
	unsigned int program = buildShaderProgram("gl_test.vert", "gl_test.frag");
	glUseProgram(program);
	
	// create and bind texture from image
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	byte* imgData = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	if (!imgData) {
		printf("Could not read image file.");
		return;
	}
	unsigned int texture = genBindStdTexture(imgData, width, height);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	int modelLoc = glGetUniformLocation(program, "model");
	int viewLoc = glGetUniformLocation(program, "view");
	int perspectiveLoc = glGetUniformLocation(program, "perspective");

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window);

		// reload color
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.raw);
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.raw);
		glUniformMatrix4fv(perspectiveLoc, 1, GL_FALSE, perspective.raw);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}