#version 430

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coord;
layout(location = 3) in vec4 p_pos_rot_size;
layout(location = 4) in vec4 p_color;
layout(location = 5) in vec2 p_texture_coord;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;

layout(location = 33) uniform vec2 frame_size;


layout(location = 0) out vec4 vtint_color;
layout(location = 2) out vec2 vtexture_coord;
layout(location = 3) out vec2 vframe_offset;
layout(location = 4) out vec2 vframe_size;

void main() {
	// p_pos_rot_size.xy = position
	// p_pos_rot_size.w = scale
	// p_pos_rot_size.z = rotation
	
	mat4 scale = mat4(
		p_pos_rot_size.w, 0, 0, 0,
		0, p_pos_rot_size.w, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	
	float cosRot = cos(p_pos_rot_size.z);
	float sinRot = sin(p_pos_rot_size.z);
	mat4 rotation = mat4(
		cosRot, sinRot, 0, 0,
		-sinRot, cosRot, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);
	
	mat4 translate = mat4(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		p_pos_rot_size.x, p_pos_rot_size.y, 0, 1
	);

	mat4 modelMatrix = translate * rotation * scale;
	
	gl_Position = persp_matrix * view_matrix * modelMatrix * position;
	vtexture_coord = texture_coord;
	vframe_offset = p_texture_coord;
	vframe_size = frame_size;
	
	vtint_color = p_color;
}