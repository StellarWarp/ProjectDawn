#version 330 core

layout (location = 0) in vec3 pos_object;

uniform mat4 MATRIX_M;
uniform mat4 MATRIX_PV;

void main()
{
    gl_Position = MATRIX_PV *MATRIX_M * vec4(pos_object,1.0f);
}