#version 330 core

uniform sampler2D rawTex;
uniform sampler2D maskTex;
uniform float angle;

in vec2 uv;
in vec3 vColor;

out vec4 FragColor;

vec3 rgb2hsv(vec3 c)
{
    vec4 K = vec4(0.0, -1.0 / 3.0, 2.0 / 3.0, -1.0);
    vec4 p = mix(vec4(c.bg, K.wz), vec4(c.gb, K.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));

    float d = q.x - min(q.w, q.y);
    float e = 1.0e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6.0 * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main()
{
	if(texture(maskTex, uv).r == 0.0f)
	{
		vec3 hsv = rgb2hsv(vColor);
		hsv.r = hsv.r + angle / 360.0;
		//hsv.g = clamp(0.0f + hsv.g, 0.0, 1.0);//Saturation
		vec3 rgb = hsv2rgb(hsv);
		FragColor = vec4(rgb, 1.0f);
	}
	else
	{
		vec3 hsv = rgb2hsv(texture(rawTex, uv).rgb);
		vec3 rgb = hsv2rgb(hsv);
		FragColor = texture(rawTex, uv);
	}
}