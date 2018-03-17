#version 430

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 2) in vec2 texture_coord;

// UNIFORM INPUTS
layout(location = 35) uniform float target_height;

// OUTPUTS
layout(location = 10) out vec2 vblur_texture_coords[11];

void main() {
	gl_Position = position;
	
	vec2 centerTexCoords = position.xy * 0.5f + 0.5f;
	float pixelSize = 1.f / target_height;
	
	for ( int i = -5; i<5; ++i) {
		vblur_texture_coords[i+5] = centerTexCoords + vec2(0, pixelSize * i);
	}
}