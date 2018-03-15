#version 430

layout(location = 0) in vec4 position;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;


void main() {
	gl_Position = persp_matrix * view_matrix * model_matrix * position;
}