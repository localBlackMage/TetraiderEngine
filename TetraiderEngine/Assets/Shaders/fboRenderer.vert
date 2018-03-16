#version 430

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texture_coord;

// UNIFORM INPUTS

// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;

// Incoming Mesh will always be a quad from -0.5 to 0.5
mat4 modelMatrix = mat4(
	2, 0, 0, 0,
	0, -2, 0, 0,
	0, 0, 1, 0,
	0, 0, 0, 1
);

void main() {
	gl_Position = modelMatrix * position;	
	vtexture_coord = texture_coord;
}