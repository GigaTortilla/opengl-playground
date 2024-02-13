#version 330 core
in vec2 textureCoords;
in vec3 ourColor;

out vec4 FragColor;  

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;
  
void main()
{
    FragColor = mix(texture(texture1, vec2(1.7 * textureCoords.s, 1.3 * textureCoords.t)), texture(texture2, vec2(1.0 - 2.0 * textureCoords.s, 2.0 * textureCoords.t)), blend) * vec4(ourColor, 1.0);
}