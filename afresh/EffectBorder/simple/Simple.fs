#version 330 core

uniform sampler2D rawTex;
uniform sampler2D maskTex;
uniform float r;
uniform float g;
uniform float b;

in vec2 uv;
in vec3 vColor;

out vec4 FragColor;

void main()
{
	if(texture(maskTex, uv).r == 0.0f)
	{
		//FragColor = vec4(r, g, b, 1.0f);
		FragColor = vec4(vColor, 1.0f);
	}
	else
	{
		FragColor = texture(rawTex, uv);
	}
	//FragColor = vec4(1.0f, aaa, 0.0f, 1.0f);
}