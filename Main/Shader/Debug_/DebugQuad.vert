#version 330 core

layout (location = 0) in vec3 pos_object;
layout (location = 1) in vec2 texcoord;

out vec2 TexCoords;

void main()
{
    TexCoords = texcoord;
    gl_Position = vec4(pos_object, 1.0);
}