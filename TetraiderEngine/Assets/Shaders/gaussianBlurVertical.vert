#version 430

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 texture_coord;


// UNIFORM INPUTS
layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;


// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;

void main() {
	vec4 P = model_matrix * position;
	gl_Position = persp_matrix * view_matrix * P;
	vtexture_coord = texture_coord;
}