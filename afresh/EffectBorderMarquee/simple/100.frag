#version 430 core

//uv坐标
in vec2 fUV;
//一维纹理
uniform sampler1D tex;

//时间因子
uniform float t;
//总的重复次数
uniform  int repeats;
//图片宽高
uniform int pic_width;
uniform int pic_height;
//彩带宽度
uniform int bell;

const float pi = 3.141592653;
//四分之一圆弧长度
float anglePixels = (pi / 2) * (2 * bell);
float wPixels = (pic_width - 4 * bell);
float hPixels = (pic_height - 4 * bell);
float totalPixels = 2 * (pic_width + pic_height) - 16 * bell + anglePixels * 4;

//沿着边缘的像素索引
float getPixelIndex(vec2 uv)
{
	int iu = int(uv.x*pic_width);
	int iv = int(uv.y*pic_height);

	float d1 = distance(vec2(iu, iv), vec2(pic_width - 2 * bell, 2 * bell));
	float d2 = distance(vec2(iu, iv), vec2(pic_width - 2 * bell, pic_height - 2 * bell));
	float d3 = distance(vec2(iu, iv), vec2(2 * bell, pic_height - 2 * bell));
	float d4 = distance(vec2(iu, iv), vec2(2 * bell, 2 * bell));

	if (iu >= 2 * bell && iu <= pic_width - 2 * bell && iv <= bell)
	{
		float accumulate = 0 * wPixels + 0 * hPixels + 0 * anglePixels;
		return accumulate + iu - 2 * bell;
	}
	else if (iu > pic_width - 2 * bell && iv < 2 * bell && d1>bell && d1 < 2 * bell)
	{
		float accumulate = 1 * wPixels + 0 * hPixels + 0 * anglePixels;
		float angle = atan(1.0*(iv - 2 * bell) / (iu - (pic_width - 2 * bell)));
		angle = pi / 2 + angle;
		return accumulate + angle*(2 * bell);
	}
	else if (iv >= 2 * bell && iv <= pic_height - 2 * bell && iu >= pic_width - bell)
	{
		float accumulate = 1 * wPixels + 0 * hPixels + 1 * anglePixels;
		return accumulate + iv - 2 * bell;
	}
	else if (iu > pic_width - 2 * bell && iv > pic_height - 2 * bell && d2 > bell && d2 < 2 * bell)
	{
		float accumulate = 1 * wPixels + 1 * hPixels + 1 * anglePixels;
		float angle = atan(1.0*(iv - (pic_height - 2 * bell)) / (iu - (pic_width - 2 * bell)));
		return accumulate + angle*(2 * bell);
	}
	else if (iu >= 2 * bell && iu <= pic_width - 2 * bell && iv >= pic_height - bell)
	{
		float accumulate = 1 * wPixels + 1 * hPixels + 2 * anglePixels;
		return accumulate + (pic_width - 2 * bell - iu);
	}
	else if (iu<2 * bell && iv > pic_height - 2 * bell  && d3 > bell && d3 < 2 * bell)
	{
		float accumulate = 2 * wPixels + 1 * hPixels + 2 * anglePixels;
		float angle = atan(1.0*(iv - (pic_height - 2 * bell))/(iu - 2 * bell));
		angle = pi / 2 + angle;
		return accumulate + angle*(2 * bell);
	}
	else if (iv >= 2 * bell && iv <= pic_height - 2 * bell && iu <= bell)
	{
		float accumulate = 2 * wPixels + 1 * hPixels + 3 * anglePixels;
		return accumulate + (pic_height - 2 * bell - iv);
	}
	else if (iu < 2 * bell && iv < 2 * bell && d4 > bell && d4 < 2 * bell)
	{
		float accumulate = 2 * wPixels + 2 * hPixels + 3 * anglePixels;
		float angle = atan(1.0*(iv - 2 * bell) / (iu - 2 * bell));
		return accumulate + angle*(2 * bell);
	}
	else
	{
		return -1;
	}
}

void main()
{
	float pixelIndex = getPixelIndex(fUV);
	if (pixelIndex == -1)
	{
		discard;
	}

	gl_FragColor = texture(tex, repeats*pixelIndex / totalPixels + t);
}