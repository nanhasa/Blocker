#version 330 core

in vec3 color;
in vec2 UVCoord;

out vec3 FragColor;

uniform sampler2D Texture;

void main()
{
    FragColor = texture(Texture, UVCoord).rgb;
}