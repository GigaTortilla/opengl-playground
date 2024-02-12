#version 330 core
out vec4 FragColor;  
in vec3 ourPosition;
in vec3 ourColor;
in vec2 textureCoords;

uniform sampler2D ourTexture;
  
void main()
{
    FragColor = texture(ourTexture, textureCoords) * vec4(ourColor, 0.5) * vec4(ourPosition, 0.5);
}