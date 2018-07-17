#version 330 core
layout(location = 0) in vec3 iPos;

uniform mat4 model;
uniform mat4 projection;

void main()
{
	gl_Position = vec4(iPos.x, iPos.y, iPos.z, 1.0f);
}