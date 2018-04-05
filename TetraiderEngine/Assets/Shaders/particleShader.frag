#version 430

layout(location = 0) uniform sampler2D spriteTexture;
layout(location = 37) uniform float brightness_tint;

layout(location = 0) in vec4 vtint_color;
layout(location = 2) in vec2 vtexture_coord;
layout(location = 3) in vec2 vframe_offset;
layout(location = 4) in vec2 vframe_size;

out vec4 frag_color;

void main(void) {
	vec4 textureColor = texture(spriteTexture, ((vtexture_coord * vframe_size) + vframe_offset));
	frag_color = textureColor * vtint_color * brightness_tint;
}