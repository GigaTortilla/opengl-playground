#version 330 core
in vec2 textureCoords;
in vec3 ourColor;

out vec4 FragColor;  

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;
  
void main()
{
    FragColor = mix(texture(texture1, textureCoords), texture(texture2, textureCoords), blend) * vec4(ourColor, 1.0);
}