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
void glTest() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, texVertices, sizeof(texVertices));

	// rectangle EBO
	unsigned int EBO = genBindEBO(texVertices, texIndices, sizeof(texIndices));

	// build and use shader program
	char vertPath[256];
	getcwd(vertPath, sizeof(vertPath));
	strcat(vertPath, "\\shaders\\basic.vert");
	char fragPath[256];
	getcwd(fragPath, sizeof(fragPath));
	strcat(fragPath, "\\shaders\\basic.frag");
	unsigned int program = buildShaderProgram(vertPath, fragPath);
	glUseProgram(program);

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
	
	// load images
	stbi_set_flip_vertically_on_load(true);
	int width1, width2, height1, height2, nrChannels1, nrChannels2;
	unsigned char* imgData1 = stbi_load("walterwhite.jpg", &width1, &height1, &nrChannels1, 0);
	unsigned char* imgData2 = stbi_load("saulgoodman.jpg", &width2, &height2, &nrChannels2, 0);
	if (!imgData1 || !imgData2) {
		perror("Could not read image file.");
		return;
	}

	// texture generation
	unsigned int textures[2];
	glGenTextures(2, &textures);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// create texture and free data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData1);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(imgData1);

	// second texture
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// create texture and free data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData2);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(imgData2);

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

		// scuffed frame limit so my GPU doesn't explode
		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}