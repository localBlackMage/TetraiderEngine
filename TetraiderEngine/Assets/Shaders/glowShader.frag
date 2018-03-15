#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D colorTexture;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;


void main(void) {
	vec4 color = texture(colorTexture, vtexture_coord);
	float brightness = (color.r + color.g + color.b) / 3.f;

	// gradient falloff - can multiply by brightness further to make the gradient steeper
	frag_color = color * brightness; 
	
	/*
	if (brightness > brightness_threshold)
		frag_color = color;
	else
		frag_color = vec4(0);
	*/
}