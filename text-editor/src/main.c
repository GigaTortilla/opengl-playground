#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glad/glad.h>
#include <glfw3.h>
#include <unistd.h>
#include <stdbool.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void create_test_file(char** argv) {
	char path[1000];
	getcwd(path, sizeof(path));
	strcat(path, "\\test.txt");

	FILE* file = fopen(path, "w");
	char* test_text = "Hello World!\nThis is a second line.";
	fwrite(test_text, 1, strlen(test_text), file);
	fclose(file);
	
	free(argv[1]);
	argv[1] = malloc(strlen(path));
	strcpy(argv[1], path);
}

int main(int argc, char** argv) {
	// create test file and pass into first argument if not specified
	//if (argc == 1)
	//	create_test_file(argv);
	//
	//// get file from first argument
	//FILE* file = fopen(argv[1], "r");
	//
	//// read file into buffer and print content
	//char contents[1024] = { 0 };
	//fread(contents, 1024, 1, file);
	//fclose(file);
	//printf("Contents:\n%s\n", contents);

	// init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE, 3);
	
	// create window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		printf("Failed to create GLFW window\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n") ;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up glfw resources
	glfwTerminate();
	return 0;
}