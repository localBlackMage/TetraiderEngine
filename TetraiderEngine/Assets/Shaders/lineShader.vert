#version 410

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 texture_coord;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform vec4 color;

out vec4 vcolor;

void main() {
	gl_Position = persp_matrix * view_matrix * model_matrix * position;
	vcolor = color;
}