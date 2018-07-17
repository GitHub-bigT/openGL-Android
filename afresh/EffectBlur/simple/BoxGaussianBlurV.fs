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
	for(int iy=-r; iy<(r+1); iy++) 
	{
		vec2 offset = vec2(0.0f, 1.0f / gaussian_height * iy);
		col += texture(tex, uv + offset);
    }
	col = col / (r+r+1.0f);

	return col;
}