#version 330 core

uniform sampler2D tex;

in vec2 uv;
out vec4 FragColor;

vec4 GaussianBlur();

void main()
{
	gl_FragDepth = 0.5;
	FragColor = texture(tex, uv);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}