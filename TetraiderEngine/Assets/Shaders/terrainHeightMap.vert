#version 430

const int MAX_LIGHTS = 20;

// NON-UNIFORM INPUTS
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec4 tangent;
layout(location = 3) in vec4 bitangent;
layout(location = 4) in vec2 texture_coord;


// UNIFORM INPUTS
layout(location = 2) uniform sampler2D heightMap;

layout(location = 10) uniform mat4 persp_matrix;
layout(location = 11) uniform mat4 view_matrix;
layout(location = 12) uniform mat4 model_matrix;
layout(location = 13) uniform mat4 normal_matrix;
layout(location = 14) uniform vec4 camera_position;

layout(location = 46) uniform float maxHeight;
layout(location = 47) uniform bool lit;
layout(location = 50) uniform vec4 l_pos_dist[MAX_LIGHTS];


// OUTPUTS
layout(location = 2) out vec2 vtexture_coord;
layout(location = 5) out vec4 vnormal_vector;
layout(location = 6) out vec4 vtangent_vector;
layout(location = 7) out vec4 vbitangent_vector;
layout(location = 8) out vec4 vview_vector;

layout(location = 10) out vec4 vl_lightVector;
layout(location = 11) out mat3 TBN;

void main() {
	float heightMapValue = texture(heightMap, texture_coord).r;
	mat4 heightMat = mat4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,heightMapValue * maxHeight,0,1
	);
	vec4 P = heightMat * model_matrix * position;
	gl_Position = persp_matrix * view_matrix * P;
	vtexture_coord = texture_coord;

	vnormal_vector = normal_matrix * normal;
	vtangent_vector = normal_matrix * tangent;
	vbitangent_vector = normal_matrix * bitangent;
	vview_vector = normalize(camera_position - P);
	
	
	vec3 T = normalize(vec3(model_matrix * tangent));
	vec3 B = normalize(vec3(model_matrix * bitangent));
	vec3 N = normalize(vec3(model_matrix * normal));
	TBN = mat3(T, B, N);
	
	
	
	vl_lightVector = vec4(0,100,100, 1) - P;
}