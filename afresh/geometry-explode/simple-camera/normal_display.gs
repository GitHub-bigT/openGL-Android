#version 330 core

layout(triangles) in;
layout(line_strip, max_vertices = 6) out;

in vec3 vNormal[];

const float MAGNITUDE = 1.0f;

vec3 GetNormal()
{
	vec3 a = vec3(gl_in[0].gl_Position - gl_in[1].gl_Position);
	vec3 b = vec3(gl_in[2].gl_Position - gl_in[1].gl_Position);
	return normalize(cross(a, b));
}

void GenerateDisplayNormal(int index)
{
	gl_Position = gl_in[index].gl_Position;
	EmitVertex();

	gl_Position = gl_in[index].gl_Position + vec4(vNormal[index], 0.0f) * MAGNITUDE;
	EmitVertex();

	EndPrimitive();
}

void main()
{
	GenerateDisplayNormal(0);
	GenerateDisplayNormal(1);
	GenerateDisplayNormal(2);
}