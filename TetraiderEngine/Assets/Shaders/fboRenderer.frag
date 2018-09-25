#version 430

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D firstTexture;
layout(location = 1) uniform sampler2D secondTexture;
layout(location = 2) uniform sampler2D thirdTexture;
layout(location = 3) uniform sampler2D fourthTexture;
layout(location = 4) uniform int texturesToRead;

// NON-UNIFORM INPUTS
layout(location = 2) in vec2 vtexture_coord;

// OUTPUTS
out vec4 frag_color;

vec4 colorAbeforeColorB(vec4 colorA, vec4 colorB) {
	return length(colorA) != 0 || colorA == vec4(0,0,0,1) ? colorA : colorB;
}

void main(void) {
	vec4 color1 = texture(firstTexture, vtexture_coord);
	//vec4 color2 = texture(secondTexture, vtexture_coord);
	//vec4 color3 = texture(thirdTexture, vtexture_coord);
	//vec4 color4 = texture(fourthTexture, vtexture_coord);
	vec4 finalColor = color1;
	
	/*
	if (texturesToRead >= 2)
		finalColor = colorAbeforeColorB(color2, color1);
	if (texturesToRead >= 3)
		finalColor = colorAbeforeColorB(color3, color2);
	if (texturesToRead >= 4)
		finalColor = colorAbeforeColorB(color4, color3);
	*/
	
	/*
	frag_color.r = vtexture_coord.x;
	frag_color.g = vtexture_coord.y;
	frag_color.b = 0;
	frag_color.a = 1;
	*/
	
	frag_color = finalColor;
}