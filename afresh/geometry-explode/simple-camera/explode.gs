#version 330 core

in vec2 TexCoords[];
out vec2 gTexCoords;

uniform float time;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

vec3 getNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 100.0f;
	vec3 direction = normal * ((sin(time) + 1.0f) / 2.0f) * magnitude;
	return position + vec4(direction, 0.0f);
}

void main()
{
	gl_Position = explode(gl_in[0].gl_Position, getNormal());
	gTexCoords = TexCoords[0];
	EmitVertex();

	gl_Position = explode(gl_in[1].gl_Position, getNormal());
	gTexCoords = TexCoords[1];
	EmitVertex();

	gl_Position = explode(gl_in[2].gl_Position, getNormal());
	gTexCoords = TexCoords[2];
	EmitVertex();

	EndPrimitive();
}