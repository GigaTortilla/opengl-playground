#ifndef MYSHADERS_H
#define MYSHADERS_H

#define INFO_LOG_SIZE 512
#define PATH_SIZE 1024

char *readShaderFileFromPath(const char *path);
unsigned int compileVertexShader(const char *vertexShaderSource);
unsigned int compileFragmentShader(const char *fragmentShaderSource);
unsigned int buildShaderProgram(const char *vertexShaderFileName, const char *fragmentShaderFileName);

#endif