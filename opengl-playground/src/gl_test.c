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
#include <camera.h>

/////////////////////////////////////////////////////////////////////////////////
// Globals

// cam has to be global to be available to callback functions
Camera cam = {
	.front = { 0.0f, 0.0f, -1.0f },
	.pos = { 0.0f, 0.0f, 3.0f },
	.up = { 0.0f, 1.0f, 0.0f },
	.speed = 2.5f,
	.pitch = 0.0f,
	.yaw = -90.0f,
	.fov = 45.0f,
	.sens = 0.1f,
	.lastMouseX = 300.0f,
	.lastMouseY = 400.0f,
	.firstMouse = true
};

/////////////////////////////////////////////////////////////////////////////////
// Callbacks

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (cam.firstMouse) {
		cam.lastMouseX = xpos;
		cam.lastMouseY = ypos;
		cam.firstMouse = false;
	}

	float xOffset = xpos - cam.lastMouseX;
	float yOffset = cam.lastMouseY - ypos;
	cam.lastMouseX = xpos;
	cam.lastMouseY = ypos;

	xOffset *= cam.sens;
	yOffset *= cam.sens;
	
	cam.yaw += xOffset;
	cam.pitch += yOffset;

	// limit pitch
	if (cam.pitch > 89.0f)
		cam.pitch = 89.0f;
	if (cam.pitch < -89.0f)
		cam.pitch = -89.0f;
}
void scrollCallBack(GLFWwindow* window, double xoffset, double yoffset) {
	cam.fov -= (float)yoffset;
	if (cam.fov < 1.0f)
		cam.fov = 1.0f;
	if (cam.fov > 75.0f)
		cam.fov = 75.0f;
}

/////////////////////////////////////////////////////////////////////////////////
// Utils

void processInput(GLFWwindow* window, float frameDelta) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	cam_updatePosFps(&cam, window, frameDelta);
	cam_updateMouse(&cam);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord
	glVertexAttribPointer(2, 2, GL_FLOAT, false, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}
unsigned int genBindStdTexture(byte* imgData, int width, int height) {
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// texture wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// texture filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// create texture and free data
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imgData);
	glGenerateMipmap(GL_TEXTURE_2D);
	free(imgData);
	return texture;
}
float updateFrameDelta(float* lastFrame) {
	// update frame time
	float currFrame = glfwGetTime();
	float deltaTime = currFrame - *lastFrame;
	*lastFrame = currFrame;
	return deltaTime;
}

/////////////////////////////////////////////////////////////////////////////////
// Test Programs

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
		processInput(window, 0.0f);

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
		processInput(window, 0.0f);

		// reload color
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// first texture
		double time = 0.0 - glfwGetTime();
		mat4s mat1 = glms_translate_make(trans_vec1);
		mat1 = glms_rotate(mat1, time, Z_AXIS);
		glUniformMatrix4fv(transformLoc, 1, false, mat1.raw);
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
		glUniformMatrix4fv(transformLoc, 1, false, mat2.raw);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[1]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);
		glfwPollEvents();

		// scuffed frame limiter
		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void render_rectangle_3d() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// matrices
	mat4s perspective = glms_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	mat4s model = glms_rotate_make(glm_rad(-55.0f), X_AXIS);
	mat4s view = glms_translate_make((vec3s){ 0.0f, 0.0f, -3.0f });
	mat4s mvp = glms_mul(glms_mul(perspective, view), model);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	genBindVAO(VBO, texVertices, sizeof(texVertices));

	// rectangle EBO
	unsigned int EBO = genBindEBO(texVertices, texIndices, sizeof(texIndices));

	textureRectanglePointerArithmetic();

	// shader program
	unsigned int program = buildShaderProgram("render_rectangle_3d.vert", "render_rectangle_3d.frag");
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

	unsigned int mvpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
	
	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window, 0.0f);

		// reload color
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void spin_cube() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	genBindVAO(VBO, cubeVertices, sizeof(cubeVertices));

	// pointer arithmetic for attributes
	// pos
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// shader program
	unsigned int program = buildShaderProgram("spin_cube.vert", "spin_cube.frag");
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

	int modelLoc = glGetUniformLocation(program, "model");
	int viewLoc = glGetUniformLocation(program, "view");
	int perspectiveLoc = glGetUniformLocation(program, "perspective");
	
	// matrices
	mat4s perspective = glms_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	mat4s view = glms_translate_make((vec3s) { 0.0f, 0.0f, -3.0f });
	glUniformMatrix4fv(viewLoc, 1, false, view.raw);
	glUniformMatrix4fv(perspectiveLoc, 1, false, perspective.raw);

	// enable depth buffer
	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window, 0.0f);

		// reload color and depth buffer
		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// recalculate rotation per frame
		mat4s model = glms_rotate_make((float)glfwGetTime() * glm_rad(50.0f), (vec3s){ 0.5f, 1.0f, 0.0f });
		glUniformMatrix4fv(modelLoc, 1, false, model.raw);
		
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void spin_10_cubes() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, cubeVertices, sizeof(cubeVertices));

	// pointer arithmetic
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// shader program
	unsigned int program = buildShaderProgram("spin_cube.vert", "spin_cube.frag");
	glUseProgram(program);

	// load image
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	byte* imgData = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	if (!imgData) {
		printf("Could not load image.");
		return;
	}
	unsigned int texture = genBindStdTexture(imgData, width, height);

	unsigned int viewLoc = glGetUniformLocation(program, "view");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int perspectiveLoc = glGetUniformLocation(program, "perspective");

	// matrices
	mat4s perspective = glms_perspective(glm_rad(55.0f), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	mat4s view = glms_translate_make((vec3s){ 0.0f, 0.0f, -3.0f });
	glUniformMatrix4fv(perspectiveLoc, 1, false, perspective.raw);
	glUniformMatrix4fv(viewLoc, 1, false, view.raw);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window)) {
		processInput(window, 0.0f);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(vec3s); i++) {
			mat4s model = glms_translate_make((vec3s){ cubePositions[i * 3], cubePositions[i * 3 + 1], cubePositions[i * 3 + 2] });
			model = glms_rotate(model, (float)(i + 1) / 10.0f * glfwGetTime(), (vec3s) { 0.2f + 0.1f * i, 0.1f, 0.1f * i });
			glUniformMatrix4fv(modelLoc, 1, false, model.raw);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void rotate_scene() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, cubeVertices, sizeof(cubeVertices));

	// pointer arithmetic
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// shader program
	unsigned int program = buildShaderProgram("spin_cube.vert", "spin_cube.frag");
	glUseProgram(program);

	// load image
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	byte* imgData = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	if (!imgData) {
		printf("Could not load image.");
		return;
	}
	unsigned int texture = genBindStdTexture(imgData, width, height);

	unsigned int viewLoc = glGetUniformLocation(program, "view");
	unsigned int modelLoc = glGetUniformLocation(program, "model");
	unsigned int perspectiveLoc = glGetUniformLocation(program, "perspective");

	// matrices
	mat4s perspective = glms_perspective(glm_rad(45.0f), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
	glUniformMatrix4fv(perspectiveLoc, 1, false, perspective.raw);

	// enable z-buffering
	glEnable(GL_DEPTH_TEST);

	// render loop
	while (!glfwWindowShouldClose(window)) {
		processInput(window, 0.0f);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// draw all cubes
		for (int i = 0; i < sizeof(cubePositions) / sizeof(vec3s); i++) {
			// translate cube inside world
			mat4s model = glms_translate_make((vec3s) { cubePositions[i * 3], cubePositions[i * 3 + 1], cubePositions[i * 3 + 2] });
			model = glms_rotate(model, 20.0f * (i + 1), (vec3s) { 0.2f, 0.5f, 0.2f });

			// camera setup
			const float radius = 10.0f;
			double time = glfwGetTime();
			float camX = sin(time) * radius;
			float camZ = cos(time) * radius;
			mat4s view = glms_lookat((vec3s) { camX, 0.0f, camZ }, glms_vec3_zero(), Y_AXIS);

			// uniforms
			glUniformMatrix4fv(viewLoc, 1, false, view.raw);
			glUniformMatrix4fv(modelLoc, 1, false, model.raw);

			// draw
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void free_movement() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	// capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set callbacks
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallBack);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	unsigned int VAO = genBindVAO(VBO, cubeVertices, sizeof(cubeVertices));

	// pointer arithmetic
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// shader program
	unsigned int program = buildShaderProgram("spin_cube_mvp.vert", "spin_cube.frag");
	glUseProgram(program);

	// load image
	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	byte* imgData = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	if (!imgData) {
		printf("Could not load image.");
		return;
	}
	unsigned int texture = genBindStdTexture(imgData, width, height);

	unsigned int mvpLoc = glGetUniformLocation(program, "mvp");

	// enable z-buffering
	glEnable(GL_DEPTH_TEST);

	float lastFrame = 0.0f;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float frameDelta = updateFrameDelta(&lastFrame);
		processInput(window, frameDelta);

		glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// projection matrix
		mat4s perspective = glms_perspective(glm_rad(cam.fov), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
		
		// draw all cubes
		for (int i = 0; i < sizeof(cubePositions) / sizeof(vec3s); i++) {
			// translate cube inside world
			mat4s model = glms_translate_make((vec3s) { cubePositions[i * 3], cubePositions[i * 3 + 1], cubePositions[i * 3 + 2] });
			model = glms_rotate(model, 20.0f * (i + 1), (vec3s) { 0.2f, 0.5f, 0.2f });

			// camera setup
			mat4s view = glms_lookat(cam.pos, glms_vec3_add(cam.pos, cam.front), Y_AXIS);

			// calc and send mvp
			mat4s mvp = glms_mat4_mul(glms_mat4_mul(perspective, view), model);
			glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);

			// draw
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

		Sleep(1);
	}

	// clean up glfw resources
	glfwTerminate();
}
void xz_plane_movement() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	// capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set callbacks
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallBack);

	unsigned int VBOs[2];
	glGenBuffers(2, &VBOs);

	// cube
	unsigned int VAOcube = genBindVAO(VBOs[0], cubeVertices, sizeof(cubeVertices));
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// xzPlane
	unsigned int VAOxzplane = genBindVAO(VBOs[1], xzPlaneVertices, sizeof(xzPlaneVertices));
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int planeProgram = buildShaderProgram("fps_movement.vert", "fps_movement.frag");
	unsigned int texProgram = buildShaderProgram("spin_cube_mvp.vert", "spin_cube.frag");

	// texture
	stbi_set_flip_vertically_on_load(true);
	unsigned int width, height, nrChannels;
	byte* imgData = stbi_load("images\\dejavu.jpg", &width, &height, &nrChannels, 0);
	if (!imgData) {
		printf("Could not load image.");
		return;
	}
	unsigned int texture = genBindStdTexture(imgData, width, height);
	glActiveTexture(GL_TEXTURE0);

	// mvp locations
	unsigned int mvpLoc = glGetUniformLocation(planeProgram, "mvp");

	// z buffer
	glEnable(GL_DEPTH_TEST);

	// render variables
	float lastFrame = 0.0f;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float frameDelta = updateFrameDelta(&lastFrame);
		processInput(window, frameDelta);

		// update color
		glClearColor(0.1f, 0.4f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// mvp
		mat4s model = glms_mat4_identity();
		mat4s view = glms_lookat(cam.pos, glms_vec3_add(cam.pos, cam.front), cam.up);
		mat4s perspective = glms_perspective(glm_rad(cam.fov), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
		mat4s mvp = glms_mul(glms_mul(perspective, view), model);

		// cube
		glUseProgram(texProgram);
		glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
		glBindVertexArray(VAOcube);
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		// plane
		glUseProgram(planeProgram);
		glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
		glBindVertexArray(VAOxzplane);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glfwPollEvents();
		glfwSwapBuffers(window);

		Sleep(1);
	}

	glfwTerminate();
}
void gl_test() {
	GLFWwindow* window = initWindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	// capture cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// set callbacks
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetScrollCallback(window, scrollCallBack);

	unsigned int VBOs[2];
	glGenBuffers(2, &VBOs);

	// cube
	unsigned int VAOcube = genBindVAO(VBOs[0], cubeVertices, sizeof(cubeVertices));
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, false, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// xzPlane
	unsigned int VAOxzplane = genBindVAO(VBOs[1], xzPlaneTexVertices, sizeof(xzPlaneTexVertices));
	textureRectanglePointerArithmetic();

	unsigned int planeProgram = buildShaderProgram("render_rectangle_3d.vert", "render_rectangle_3d.frag");
	unsigned int cubeProgram = buildShaderProgram("spin_cube_mvp.vert", "spin_cube.frag");

	// textures
	stbi_set_flip_vertically_on_load(true);
	unsigned int dejavuW, dejavuH, dejavuChannels;
	unsigned int grassTopW, grassTopH, grassTopChannels;
	unsigned int grassSideW, grassSideH, grassSideChannels;
	byte* imgDejavu = stbi_load("images\\dejavu.jpg", &dejavuW, &dejavuH, &dejavuChannels, 0);
	byte* imgGrassTop = stbi_load("images\\grass.jpg", &grassTopW, &grassTopH, &grassTopChannels, 0);
	byte* imgGrassSide = stbi_load("images\\grass_side.png", &grassSideW, &grassSideH, &grassSideChannels, 0);
	if (!imgDejavu || !imgGrassTop || !imgGrassSide) {
		printf("Could not load image.");
		return;
	}
	unsigned int dejavuTex = genBindStdTexture(imgDejavu, dejavuW, dejavuH);
	unsigned int grassTopTex = genBindStdTexture(imgGrassTop, grassTopW, grassTopH);
	unsigned int grassSideTex = genBindStdTexture(imgGrassSide, grassSideW, grassSideH);

	// uniform locations
	unsigned int mvpLoc = glGetUniformLocation(planeProgram, "mvp");

	// z buffer
	glEnable(GL_DEPTH_TEST);

	// render variables
	float lastFrame = 0.0f;

	// render loop
	while (!glfwWindowShouldClose(window)) {
		float frameDelta = updateFrameDelta(&lastFrame);
		processInput(window, frameDelta);

		// update color
		glClearColor(0.1f, 0.4f, 0.55f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// mvp
		mat4s model = glms_mat4_identity();
		mat4s view = glms_lookat(cam.pos, glms_vec3_add(cam.pos, cam.front), cam.up);
		mat4s perspective = glms_perspective(glm_rad(cam.fov), WINDOW_WIDTH / WINDOW_HEIGHT, NEAR_PLANE, FAR_PLANE);
		mat4s mvp = glms_mul(glms_mul(perspective, view), model);

		// cube
		glUseProgram(cubeProgram);
		glBindTexture(GL_TEXTURE_2D, dejavuTex);
		glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
		glBindVertexArray(VAOcube);
		glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);

		// plane
		glUseProgram(planeProgram);
		glBindTexture(GL_TEXTURE_2D, grassTopTex);
		glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
		glBindVertexArray(VAOxzplane);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		for (size_t x = 0; x < 10; x++)	{
			mvp = glms_translate(mvp, (vec3s) { 1.0f, 0.0f, 0.0f });
			int z = 0;
			for (; z < 10; z++) {
				mvp = glms_translate(mvp, (vec3s) { 0.0f, 0.0f, 1.0f });
				// grass cube
				glUseProgram(cubeProgram);
				glBindTexture(GL_TEXTURE_2D, grassSideTex);
				glUniformMatrix4fv(mvpLoc, 1, false, mvp.raw);
				glBindVertexArray(VAOcube);
				glDrawArrays(GL_TRIANGLES, 0, cubeVertexCount);
			}
			mvp = glms_translate(mvp, (vec3s) { 0.0f, 0.0f, -z });
		}

		glfwPollEvents();
		glfwSwapBuffers(window);

		Sleep(1);
	}

	glfwTerminate();
}