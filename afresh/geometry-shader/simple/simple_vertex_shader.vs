#version 330 core

layout(location = 0) in vec2 iPos;
layout(location = 1) in vec3 iColor;

out vec3 oColor;

void main()
{
	gl_Position = vec4(iPos.x, iPos.y, 0.0f, 1.0f);
	oColor = iColor;
	gl_PointSize = 10.0f;
}