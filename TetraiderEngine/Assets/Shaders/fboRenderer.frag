#version 430

// UNIFORM INPUTS
//layout(location = 0) uniform sampler2D fboTexture;
//layout(location = 1) uniform sampler2D otherTexture;

uniform sampler2D fboTexture;
uniform sampler2D otherTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	vec4 otherColor = texture(otherTexture, vtexture_coord);
	vec4 fboColor = texture(fboTexture, vtexture_coord);
	vec4 finalColor = vec4(fboColor.r, fboColor.g, fboColor.b, fboColor.a);

	frag_color = texture(fboTexture, vtexture_coord);
}