#version 330 core

uniform sampler2D tex;
uniform int mul;
const int rawWidth = 1920;
const int rawHeight = 1200;


in vec2 uv;
out vec4 FragColor;

void main()
{
	float unit_x = 1.0f / rawWidth;
	float unit_y = 1.0f / rawHeight;
	
	vec4 col = vec4(0.0f);
	for(int i = 0; i < mul; i++)
	{
		for(int j = 0; j < mul; j++)
		{
			col += texture(tex, uv + vec2(i * unit_x, j * unit_y));
		}
	};
	FragColor = col / (mul * mul);
	//FragColor = texture(tex, uv);
}