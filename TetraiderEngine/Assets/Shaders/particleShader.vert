#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texture_coord;
layout(location = 2) in vec4 p_pos_size;
layout(location = 3) in vec4 p_color;
layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;


layout(location = 0) out vec4 vtint_color;
layout(location = 2) out vec2 vtexture_coord;

void main() {
	// p_pos_size.w = scale
	mat4 modelMatrix = mat4(
		p_pos_size.w, 0, 0, 0,
		0, p_pos_size.w, 0, 0,
		0, 0, 1, 0,
		p_pos_size.x, p_pos_size.y, p_pos_size.z, 1
	);
	gl_Position = persp_matrix * view_matrix * modelMatrix * position;
	
	vtexture_coord = texture_coord;
	
	vtint_color = p_color;
}