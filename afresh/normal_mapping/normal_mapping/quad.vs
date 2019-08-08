#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 tangent;
out vec2 uv;
out vec3 worldPos;
out vec3 normal_vs;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vec3 bitangent = cross(normal, tangent); 
	vec3 T = normalize(vec3(model * vec4(tangent,   0.0)));
    vec3 B = normalize(vec3(model * vec4(bitangent, 0.0)));
    vec3 N = normalize(vec3(model * vec4(normal,    0.0)));
    TBN = mat3(T, B, N);

	normal_vs = transpose(inverse(mat3(model))) * normal;
	worldPos = (model * vec4(position, 1.0f)).xyz;
	uv = texCoords;
}