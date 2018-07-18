#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTex;
layout(location = 2) in vec3 iColor;

out vec2 uv;
out vec3 vColor;

void main()
{
	gl_Position = vec4(iPos.x, iPos.y, iPos.z, 1.0f);
	uv = iTex;
	vColor = iColor;
}