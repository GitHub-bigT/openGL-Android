#version 330 core
layout(location = 0) in vec2 iPos;

void main()
{
	gl_Position = vec4(iPos.x, iPos.y, 0.0f, 1.0f);
	gl_PointSize = 10.0f;
}