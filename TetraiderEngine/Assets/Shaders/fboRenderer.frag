#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D fboTexture;
layout(location = 1) uniform sampler2D sandTex;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	frag_color = texture(fboTexture, vtexture_coord);// + (texture(sandTex, vtexture_coord) / 4.f);
}