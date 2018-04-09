#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D brushTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	//frag_color = texture(brushTexture, vtexture_coord);	
	
	frag_color = vec4(1,0,0,1);	
}