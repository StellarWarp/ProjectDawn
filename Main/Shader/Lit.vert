#version 330 core
layout (location = 0) in vec3 pos_object;
layout (location = 1) in vec3 normal_object;
layout (location = 2) in vec2 texcoord;
layout (location = 3) in vec3 tangent_object;
layout (location = 4) in vec3 bitangent_object;

out v2f{
	vec3 pos_world;
	vec3 normal_world;
	vec4 pos_light_view;
}o;

//out vec2 TexCoord;

uniform mat4 MATRIX_M;
uniform mat4 MATRIX_PV;
uniform mat3 MATRIX_N;
uniform mat4 MATRIX_LIGHT;



void main()
{
	vec4 pos_world4 = MATRIX_M * vec4(pos_object, 1.0);
	o.pos_world = pos_world4.xyz;    
    gl_Position = MATRIX_PV * pos_world4;
	o.normal_world = MATRIX_N * normal_object;
	o.pos_light_view = MATRIX_LIGHT * pos_world4;
}