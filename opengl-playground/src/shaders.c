#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

char* readFileFromPath(const char* path) {
	FILE* f = fopen(path, "rb");
	if (!f) {
		printf("Shader source file not found at %s\n", path);
		return;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	rewind(f);
	char* string = malloc(fsize + 1);
	fread(string, fsize, 1, f);
	fclose(f);
	string[fsize] = 0;
	return string;
}
unsigned int compileVertexShader(const char* vertexShaderSource) {
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s", infoLog);
	}
	return vertexShader;
}
unsigned int compileFragmentShader(const char* fragmentShaderSource) {
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s", infoLog);
	}
	return fragmentShader;
}
unsigned int buildShaderProgram(const char* vertPath, const char* fragPath) {
	char* vertSource = readFileFromPath(vertPath);
	char* fragSource = readFileFromPath(fragPath);

	unsigned int vertShader = compileVertexShader(vertSource);
	unsigned int fragShader = compileFragmentShader(fragSource);

	unsigned int program;
	program = glCreateProgram();

	// link shaders and program
	glAttachShader(program, vertShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);

	// delete shader and free path memory
	free(vertPath);
	free(fragPath);
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	return program;
}