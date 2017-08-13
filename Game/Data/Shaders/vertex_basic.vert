#version 330 core

layout (location = 0) in vec3 iPosition_modelspace;
layout (location = 1) in vec2 iVertexUV;

out vec2 UVCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(iPosition_modelspace, 1.0);
    UVCoord = vec2(iVertexUV.x, iVertexUV.y);
}