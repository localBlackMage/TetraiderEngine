#version 430

layout(location = 31) uniform vec4 saturation_color;

out vec4 frag_color;

void main(void) {
	frag_color = saturation_color;
}