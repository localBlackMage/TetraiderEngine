#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coord;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;

layout(location = 31) uniform vec4 saturation_color;


layout(location = 1) out vec4 vsaturation_color;

void main() {
	gl_Position = persp_matrix * view_matrix * model_matrix * position;
	vsaturation_color = saturation_color;
}