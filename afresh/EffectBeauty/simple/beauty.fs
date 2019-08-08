#version 450

uniform sampler2D raw_tex;
uniform sampler2D beauty_curve_tex;

in vec2 uv;
out vec4 FragColor;

vec4 beautyEffect()
{
	vec4 col = texture(raw_tex, uv);

	float exponent = 25.0;
	vec4 center = col;
	vec4 color = vec4(0.0,0.0,0.0,0.0);
	float total = 0.0;

	vec3 yuv = col.xyz;
	yuv.y = -0.148 * center.x - 0.291 * center.y + 0.439 * center.z + 0.5;
	yuv.z = 0.439 * center.x - 0.368 * center.y - 0.071 * center.z + 0.5;

	//如果不是皮肤部分，则返回
	if(!(yuv.z >= 133.0/255.0 && yuv.z <= 173.0/255.0 && yuv.y >= 77.0/255.0 && yuv.y <= 0.5))
	{
		return col;
	}
	else
	{
		for(float x =-8.0;x<=8.0;x+=1.0)
		{
			for(float y =-8.0;y<=8.0;y+=1.0)
			{
				vec4 sample_color = texture(raw_tex, uv + vec2(x/1280.0,y/720.0));
				float weight = 1.0 - abs(dot(sample_color.rgb-center.rgb,vec3(0.25,0.25,0.25)));
				weight = pow(weight,exponent);
				color += sample_color * weight;
				total += weight;
			}
		}
	}

	vec4 final_color = color/total;
	final_color = final_color * 0.9 * 1.05 + col * 0.1;

	vec2 R = vec2(final_color.r, 0.5);
	final_color.r = texture(beauty_curve_tex, R).r;


	vec2 G = vec2(final_color.g, 0.5);
	final_color.g = texture(beauty_curve_tex, G).g;


	vec2 B = vec2(final_color.b, 0.5);
	final_color.b = texture(beauty_curve_tex, B).b;

	return clamp(final_color,0.0,1.0);
}

void main()
{
	FragColor = beautyEffect();
}