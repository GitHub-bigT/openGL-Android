#version 330 core

uniform int gaussian_width;
uniform int gaussian_height;
uniform sampler2D tex;
uniform int r;

in vec2 uv;
out vec4 FragColor;

vec4 BoxBlur();

void main()
{
	FragColor = BoxBlur();
}

vec4 BoxBlur()
{
	vec4 col = vec4(0.0);
	for(int ix=-r; ix<(r+1); ix++) 
	{
		vec2 offset = vec2(1.0f / gaussian_width * ix, 0.0f);
		col += texture(tex, uv + offset);
    }
	col = col / (r+r+1.0);

	return col;
}