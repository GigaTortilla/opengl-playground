#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <GLAD/glad.h>
#include <myshaders.h>

char *readShaderFileFromPath(const char *path)
{
    FILE *f = fopen(path, "rb");
    if(!f)
    {
        printf("Shader file could not be opened at location: %s\n", path);
        return NULL;
    }
    fseek(f, 0L, SEEK_END);
    int fileSize = ftell(f) + 1;
    rewind(f);
    char *shaderContent = calloc(fileSize, sizeof(char));
    if(!shaderContent)
    {
        printf("Memory error!\n");
        return NULL;
    }
    fread(shaderContent, sizeof(char), fileSize - 1, f);
    fclose(f);
    return shaderContent;
}

unsigned int compileVertexShader(const char *vertexShaderSource)
{
    char infoLog[INFO_LOG_SIZE];
    int success = 0;
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, INFO_LOG_SIZE, NULL, infoLog);
		printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}
    return vertexShader;
}

unsigned int compileFragmentShader(const char *fragmentShaderSource)
{
    char infoLog[INFO_LOG_SIZE];
    int success = 0;
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, INFO_LOG_SIZE, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }
    return fragmentShader;
}

unsigned int buildShaderProgram(const char *vertexShaderFileName, const char *fragmentShaderFileName)
{
    unsigned int shaderProgram = glCreateProgram();
    char infoLog[INFO_LOG_SIZE];
    int success = 0;
    // Store the absolute path to the shader files
    char vertexShaderPath[PATH_SIZE];
    char fragmentShaderPath[PATH_SIZE];

    getcwd(vertexShaderPath, PATH_SIZE);
    strcat(vertexShaderPath, "\\shaders\\");
    strcat(vertexShaderPath, vertexShaderFileName);

    getcwd(fragmentShaderPath, PATH_SIZE);
    strcat(fragmentShaderPath, "\\shaders\\");
    strcat(fragmentShaderPath, fragmentShaderFileName);

    char *vertexShaderSource = readShaderFileFromPath(vertexShaderPath);
    char *fragmentShaderSource = readShaderFileFromPath(fragmentShaderPath);

    // Creating the shaders
    unsigned int vertexShader = compileVertexShader(vertexShaderSource);
    unsigned int fragmentShader = compileFragmentShader(fragmentShaderSource);

    // Preparing and linking the shader program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, INFO_LOG_SIZE, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}