#version 150
in vec4 position;
in vec2 texture_coord;

uniform mat4 persp_matrix;
uniform mat4 view_matrix;
uniform mat4 model_matrix;
uniform mat4 normal_matrix;
uniform vec2 frame_offset;
uniform vec2 frame_size;
uniform vec4 color;

out vec4 vcolor;
out vec2 vtexture_coord;
out vec2 vframe_offset;
out vec2 vframe_size;

void main() {
	vec4 P = model_matrix * position;
	gl_Position = persp_matrix * view_matrix * P;
	vtexture_coord = texture_coord;
	vframe_offset = frame_offset;
	vframe_size = frame_size;
	vcolor = color;
}