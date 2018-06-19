#version 430

const int MAX_LIGHTS = 20;

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 bitangent;
layout(location = 4) in vec2 texture_coord;


// UNIFORM INPUTS
layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;
layout(location = 13) uniform mat4 normal_matrix;
layout(location = 14) uniform vec4 camera_position;

layout(location = 47) uniform bool lit;
layout(location = 50) uniform vec4 l_pos_dist[MAX_LIGHTS];


// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;
layout(location = 5) out vec4 vnormal_vector;
layout(location = 6) out vec4 vtangent_vector;
layout(location = 7) out vec4 vbitangent_vector;
layout(location = 8) out vec4 vview_vector;

layout(location = 10) out vec4 vl_lightVectors[MAX_LIGHTS];

void main() {
	vec4 P = model_matrix * position;
	gl_Position = persp_matrix * view_matrix * P;
	vtexture_coord = texture_coord;


	if (lit) {
		for(int i = 0; i < MAX_LIGHTS; ++i) {
			if (l_pos_dist[i].w == 0)
				vl_lightVectors[i] = vec4(0,0,0,1);
			else
				vl_lightVectors[i] = vec4(l_pos_dist[i].xyz, 1) - P;
		}
	}

	vnormal_vector = normal_matrix * normal;
	vtangent_vector = normal_matrix * tangent;
	vbitangent_vector = normal_matrix * bitangent;
	vview_vector = camera_position - P;
}