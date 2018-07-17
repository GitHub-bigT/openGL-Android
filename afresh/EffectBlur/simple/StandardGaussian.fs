#version 330 core

uniform float gaussian_width;
uniform float gaussian_height;
uniform sampler2D tex;
float PI = 3.141592653589793;
float sigma = 4.0f;

in vec2 uv;
out vec4 FragColor;

vec4 GaussianBlur();

void main()
{
	FragColor = GaussianBlur();
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

vec4 GaussianBlur()
{
	int r = int(ceil(sigma * 2.57));
	//int gaussian_radius = int(sigma * 3);
	float allWeight = 0.0f;

	vec4 col = vec4(0.0);
	for(int ix=-r; ix<(r+1); ix++) 
	{
		for(int iy=-r; iy<(r+1); iy++) 
		{
			float weight = 1.0f / (2 * PI * sigma * sigma) * exp(-(ix * ix + iy * iy) / (2 * sigma * sigma));
			vec2 offset = vec2(1.0f / gaussian_width * ix, 1.0f / gaussian_height * iy);
			col += texture(tex, uv + offset) * weight;
			allWeight += weight;
		}
    }
	col = col / allWeight;

	return col;
}