#version 330 core

uniform float gaussian_width;
uniform float gaussian_height;
uniform float sigmaSquarea;
uniform sampler2D tex;
//const float gaussian_width = 960.0f;
//const float gaussian_height = 600.0f;
float PI = 3.141592653589793;
float sigma = 0.0f;
const int n = 1;

in vec2 uv;
out vec4 FragColor;

vec4 GaussianBlur();
vec3 gaussBlur_2(vec3 scl);
vec3 boxBlur_2 (vec3 scl, int r);

void main()
{
	if(sigma > 0)
	{
		//vec3 scl = texture(tex, uv).xyz;
		//FragColor = vec4(gaussBlur_2(scl), 1.0f);
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