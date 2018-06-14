#version 330 core
layout(location = 0) in vec3 iPos;
layout(location = 1) in vec2 iTex;

out vec2 fTex;

void main()
{
	gl_Position = vec4(iPos.x, iPos.y, iPos.z, 1.0f);
	fTex = vec2(iTex.x, 1.0f - iTex.y);
}