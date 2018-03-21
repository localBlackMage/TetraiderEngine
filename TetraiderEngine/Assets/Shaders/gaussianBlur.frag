#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D gausTexture;

// NON-UNIFORM INPUTS
layout(location = 10) in vec2 vblur_texture_coords[11];

// OUTPUTS
out vec4 frag_color;

// http://dev.theomader.com/gaussian-kernel-calculator/
// Sigma 2, kernal size 11
void main(void) {
	frag_color = vec4(0,0,0,0);
	
	frag_color += texture(gausTexture, vblur_texture_coords[0]) * 0.0093;
	frag_color += texture(gausTexture, vblur_texture_coords[1]) * 0.028002;
	frag_color += texture(gausTexture, vblur_texture_coords[2]) * 0.065984;
	frag_color += texture(gausTexture, vblur_texture_coords[3]) * 0.121703;
	frag_color += texture(gausTexture, vblur_texture_coords[4]) * 0.175713;
	frag_color += texture(gausTexture, vblur_texture_coords[5]) * 0.198596;
	frag_color += texture(gausTexture, vblur_texture_coords[6]) * 0.175713;
	frag_color += texture(gausTexture, vblur_texture_coords[7]) * 0.121703;
	frag_color += texture(gausTexture, vblur_texture_coords[8]) * 0.065984;
	frag_color += texture(gausTexture, vblur_texture_coords[9]) * 0.028002;
	frag_color += texture(gausTexture, vblur_texture_coords[10]) * 0.0093;
	
	frag_color.a = 1;
}