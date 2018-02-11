#version 430

layout(location = 0) uniform sampler2D usampler;

layout(location = 0) in vec4 vtint_color;
layout(location = 2) in vec2 vtexture_coord;

out vec4 frag_color;

void main(void) {
	//frag_color = (texture(usampler, vtexture_coord) * vtint_color);
	frag_color = vtint_color;
}