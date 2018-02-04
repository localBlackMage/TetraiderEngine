#version 430

layout(location = 0) in vec4 vcolor;

out vec4 frag_color;

void main(void) {
	frag_color = vcolor;
}