#version 330 core
layout(location = 0) in vec2 iPos;
layout(location = 1) in vec3 iColor;

out vec3 oColor;

uniform vec2 offsets[100];

void main()
{
	vec2 offset = offsets[gl_InstanceID];
	gl_Position = vec4(iPos + offset, 0.0f, 1.0f);
	oColor = iColor;
}