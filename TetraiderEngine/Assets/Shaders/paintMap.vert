#version 430

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texture_coord;

// UNIFORM INPUTS
layout(location = 12) uniform mat4 model_matrix;

// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;

void main() {
	gl_Position = model_matrix * position;
	vtexture_coord = texture_coord;
}