#version 330 core

in vec2 TexCoords[];
in float RandomNum[];
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

void drawExplode()
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

vec4 decomposition(vec4 position, float random)
{
	float magnitude = 3.0f;
	if(time > random)
	{
		vec4 off_position = magnitude * vec4(1.0f, 0.0f, 0.0f, 0.0f) * (time - random);
		position = position + off_position;
	}
	return position;
}

void drawDecomposition()
{
	gl_Position = decomposition(gl_in[0].gl_Position, RandomNum[0]);
	gTexCoords = TexCoords[0];
	EmitVertex();

	gl_Position = decomposition(gl_in[1].gl_Position, RandomNum[0]);
	gTexCoords = TexCoords[1];
	EmitVertex();

	gl_Position = decomposition(gl_in[2].gl_Position, RandomNum[0]);
	gTexCoords = TexCoords[2];
	EmitVertex();


	EndPrimitive();
}

void main()
{
	//drawExplode();
	drawDecomposition();
}