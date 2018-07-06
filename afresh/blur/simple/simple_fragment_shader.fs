#version 330 core

uniform float gaussian_width;
uniform float gaussian_height;
uniform float sigmaSquarea;
uniform sampler2D tex;
//const float gaussian_width = 960.0f;
//const float gaussian_height = 600.0f;
float PI = 3.141592653589793;
float sigma = 0.0;

in vec2 uv;
out vec4 FragColor;

vec4 GaussianBlur();

void main()
{
	if(sigma > 0)
	{
		FragColor = GaussianBlur();
	}
	else
	{
		FragColor = texture(tex, uv);
	}	
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}

vec4 GaussianBlur()
{
	//int gaussian_radius = int(ceil(sigma * 2.57));
	int gaussian_radius = int(sigma * 3);
	int rowLength = gaussian_radius * 2 + 1;
	int columnLength = gaussian_radius * 2 + 1;
	float allWeight = 0.0f;

	vec4 col = vec4(0.0);
	for (int i = 0; i < rowLength * columnLength; i++)
	{
		int rowOffset = int((mod(i, rowLength))) - gaussian_radius;
		int columnOffset = gaussian_radius - i / rowLength;
		vec2 offset = vec2(1.0f / gaussian_width * rowOffset, 1.0f / gaussian_height * columnOffset);
			
		float weight = 1.0f / (2 * PI * sigma * sigma) * exp(-(rowOffset * rowOffset + columnOffset * columnOffset) / (2 * sigma * sigma));
		col += texture(tex, uv + offset) * weight;

		allWeight += weight;
	}
	col = col / allWeight;

	return col;
}