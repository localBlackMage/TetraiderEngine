#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;

layout(location = 20) uniform vec4 color;
layout(location = 21) uniform vec2 frame_offset;
layout(location = 22) uniform vec2 frame_size;


layout(location = 0) out vec4 vcolor;
layout(location = 1) out vec2 vtexture_coord;
layout(location = 2) out vec2 vframe_offset;
layout(location = 3) out vec2 vframe_size;

void main() {
	vec4 P = model_matrix * position;
	gl_Position = persp_matrix * view_matrix * P;
	vtexture_coord = texture_coord;
	vframe_offset = frame_offset;
	vframe_size = frame_size;
	vcolor = color;
}