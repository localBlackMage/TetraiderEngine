#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D textureToDraw;
layout(location = 1) uniform sampler2D maskTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	vec4 black = vec4(0,0,0,1);
	vec4 maskColor = texture(maskTexture, vtexture_coord);	
	
	if (maskColor == black)
		frag_color = vec4(0,0,0,0);
	else 
		frag_color = texture(textureToDraw, vtexture_coord);	
}