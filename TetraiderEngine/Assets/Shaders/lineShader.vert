#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;

layout(location = 20) uniform vec4 color;


layout(location = 0) out vec4 vcolor;

void main() {
	gl_Position = persp_matrix * view_matrix * model_matrix * position;
	vcolor = color;
}