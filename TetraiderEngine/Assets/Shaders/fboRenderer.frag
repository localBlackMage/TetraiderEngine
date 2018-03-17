#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D fboTexture;
layout(location = 1) uniform sampler2D otherTexture;

//uniform sampler2D fboTexture;
//uniform sampler2D otherTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	/*
	vec4 invert = vec4(1,1,1,2);
	vec4 fboColor = texture(fboTexture, vtexture_coord);
	vec4 otherColor = texture(otherTexture, vtexture_coord);
	if (vtexture_coord.x < 0.5f)
		frag_color = invert - fboColor;
	else
		frag_color = otherColor;
	*/
	frag_color = texture(fboTexture, vtexture_coord);
		
	//mix(fboColor, otherColor, 0.1);
}