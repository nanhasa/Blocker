#version 330 core

layout (location = 0) in vec3 iPosition_modelspace;
layout (location = 1) in vec3 iColor;
layout (location = 2) in vec2 iVertexUV;

out vec3 color;
out vec2 UVCoord;

void main()
{
    gl_Position = vec4(iPosition_modelspace, 1.0);
    color = iColor;
    UVCoord = iVertexUV;
}