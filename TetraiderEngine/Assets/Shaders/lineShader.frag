#version 430

layout(location = 1) in vec4 vsaturation_color;

out vec4 frag_color;

void main(void) {
	frag_color = vsaturation_color;
}