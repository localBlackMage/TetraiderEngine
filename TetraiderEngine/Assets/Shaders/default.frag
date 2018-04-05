#version 430

const int MAX_LIGHTS = 20;

// UNIFORM INPUTS
layout(location = 0) uniform sampler2D spriteTexture;

layout(location = 40) uniform vec3 ambient_global_color;
layout(location = 41) uniform vec3 ambient_color;
//layout(location = 42) uniform vec3 diffuse_color;
layout(location = 43) uniform vec3 specular_color;
//layout(location = 44) uniform float spec_coeff;

layout(location = 47) uniform bool lit;
layout(location = 48) uniform float l_a;
layout(location = 49) uniform float l_b;
layout(location = 50) uniform vec4 l_pos_dist[MAX_LIGHTS];
layout(location = 70) uniform vec4 l_color[MAX_LIGHTS]; 	// 50 + MAX_LIGHTS
layout(location = 90) uniform vec2 l_ab[MAX_LIGHTS];		// 50 + MAX_LIGHTS + MAX_LIGHTS


// NON-UNIFORM INPUTS
layout(location = 0) in vec4 vtint_color;
layout(location = 1) in vec4 vsaturation_color;
layout(location = 2) in vec2 vtexture_coord;
layout(location = 3) in vec2 vframe_offset;
layout(location = 4) in vec2 vframe_size;
layout(location = 5) in vec4 vnormal_vector;
layout(location = 6) in vec4 vview_vector;

layout(location = 10) in vec4 vl_lightVectors[MAX_LIGHTS];


// OUTPUTS
out vec4 frag_color;

float falloff(float dist, float a, float b) {
	return 1.f / (1.f + a * dist + b * dist * dist);
}

void main(void) {
	vec2 offsetCoords = (vtexture_coord * vframe_size) + vframe_offset;
	vec4 m = normalize(vnormal_vector);
	vec4 v = normalize(vview_vector);
	vec3 lightColor = vec3(0,0,0);

	if (lit){
		for(int i = 0; i < MAX_LIGHTS; ++i) {
			if(vl_lightVectors[i].w == 1)
				continue;
			float d = length(vl_lightVectors[i]);

			vec4 L = normalize(vl_lightVectors[i]);
			d /= 143.108f; // As d is in pixels, we want it in world units. A single world unit is the diagonal of a cell
			lightColor += (max(dot(m,L),0) * l_color[i].xyz) * falloff(d, l_ab[i].x, l_ab[i].y);
		}
		lightColor += ambient_global_color;
	}
	else
		lightColor = vec3(1,1,1);

	frag_color = ((texture(spriteTexture, offsetCoords) * vtint_color) + vsaturation_color) * vec4(lightColor,1);
}