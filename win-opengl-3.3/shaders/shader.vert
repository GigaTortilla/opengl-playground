#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

out vec3 ourPosition;
// out vec3 ourColor; // output a color to the fragment shader
uniform float ourOffsetX;    // offsets the vertex horizontally

void main()
{
    gl_Position = vec4(aPos.x + ourOffsetX, -aPos.y - ourOffsetX, aPos.z, 1.0);
    // ourColor = aColor; // set ourColor to the input color we got from the vertex data
    ourPosition = vec3(aPos.x + ourOffsetX + 0.5, aPos.y + 0.5, 0.2);
} 