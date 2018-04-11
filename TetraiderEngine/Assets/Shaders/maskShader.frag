#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D brushTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	vec4 brushColor = texture(brushTexture, vtexture_coord);
	
	if (brushColor.a != 0.0)
		frag_color = vec4(1,1,1,1);
	else
		frag_color = vec4(0,0,0,0);
}