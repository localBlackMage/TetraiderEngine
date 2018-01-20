#version 150

uniform sampler2D usampler;

in vec2 vtexture_coord;
in vec2 vframe_offset;
in vec2 vframe_size;
in vec4 vcolor;

out vec4 frag_color;

void main(void) {
	vec2 offsetCoords = (vtexture_coord * vframe_size) + vframe_offset;
	frag_color = texture(usampler, offsetCoords) * vcolor;
}