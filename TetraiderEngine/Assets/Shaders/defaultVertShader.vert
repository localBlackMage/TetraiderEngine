#version 150
in vec4 position;
in vec2 texture_coord;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;

void main() {
	gl_Position = persp_matrix * view_matrix * model_matrix * position;
}