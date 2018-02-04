#version 430

uniform sampler2D usampler;

layout(location = 0) in vec4 vcolor;
layout(location = 1) in vec2 vtexture_coord;
layout(location = 2) in vec2 vframe_offset;
layout(location = 3) in vec2 vframe_size;

out vec4 frag_color;

void main(void) {
	vec2 offsetCoords = (vtexture_coord * vframe_size) + vframe_offset;
	frag_color = texture(usampler, offsetCoords) * vcolor;
}