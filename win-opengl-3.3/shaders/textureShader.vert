#version 330 core
layout (location = 0) in vec3 aPos;   		// the position variable has attribute position 0
layout (location = 1) in vec3 aColor; 		// the color variable has attribute position 1
layout (location = 2) in vec2 aTexCoords;	// texture coordinates have the attribute position 2

out vec3 ourPosition;
out vec3 ourColor;
out vec2 textureCoords;
uniform float ourOffsetX;    // offsets the vertex horizontally

void main()
{
    gl_Position = vec4(aPos.x + ourOffsetX, -aPos.y - ourOffsetX * 1.25, aPos.z, 1.0);
    ourPosition = vec3(aPos.x + ourOffsetX + 0.5, aPos.y + 0.5 - ourOffsetX, 0.2);
	textureCoords = aTexCoords;
	ourColor = aColor;
} 