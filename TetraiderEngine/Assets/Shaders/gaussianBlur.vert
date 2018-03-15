#version 430

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texture_coord;


// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;

void main() {
	gl_Position = position;
	vtexture_coord = texture_coord;
}