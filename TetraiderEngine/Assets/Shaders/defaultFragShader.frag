#version 430

layout(location = 0) uniform sampler2D usampler;

layout(location = 0) in vec4 vtint_color;
layout(location = 1) in vec4 vsaturation_color;
layout(location = 2) in vec2 vtexture_coord;
layout(location = 3) in vec2 vframe_offset;
layout(location = 4) in vec2 vframe_size;

out vec4 frag_color;

void main(void) {
	vec2 offsetCoords = (vtexture_coord * vframe_size) + vframe_offset;
	frag_color = (texture(usampler, offsetCoords) * vtint_color) + vsaturation_color;
}