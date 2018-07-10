#include <iostream>
#include <string>
#include <math.h>
#include <windows.h>
#include <vector>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
//inner
#include "ShaderUtil.h"
#include "ReadFileUtil.h"

//const
const int windowWidth = 960;
const int windowHeight = 600;
float PI = 3.141592653589793;
//common property
GLuint shaderProgram;

//common Util
#define SHADER_DEBUG 0;
ShaderUtil bigTShaderUtil;
TextFile bigTReadFileUtil;

//Triangle
#define TRIANGLE_DEBUG 0;
GLuint triangleVAO;
GLuint triangleVBO;
GLuint triangleTex;
GLfloat triangleVertices[] =
{
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f
};

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int w, int h);
void process_input(GLFWwindow *window);

//render
void initGL();
GLuint initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource);
void initTriangle();
void initTexture();
void gaussianBlur(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int sigma);
void draw_scene(GLFWwindow *window);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "bigT-triangle", NULL, NULL);
	if (window == NULL)
	{
		printf("GLFW Window == NULL\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//registe callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD Init Failed\n");
		return -1;
	}

	//initGL
	initGL();

	while (!glfwWindowShouldClose(window))
	{
		//event
		glfwPollEvents();
		process_input(window);

		//render
		draw_scene(window);
	}

	//release glfw
	glfwTerminate();

	return 0;
}

void timer_start(FILETIME *start)
{
	GetSystemTimeAsFileTime(start);
}

int64_t timer_elapsed_msec(FILETIME *start)
{
	FILETIME now;
	GetSystemTimeAsFileTime(&now);
	int64_t elapsed = ((((int64_t)now.dwHighDateTime) << 32) + (int64_t)now.dwLowDateTime) -
		((((int64_t)start->dwHighDateTime) << 32) + (int64_t)start->dwLowDateTime);
	return elapsed / 10000;
}

void GaussianBlur(unsigned char* img, int w, int h, int channels, int radius)
{
	radius = glm::min(glm::max(1, radius), 248);
	unsigned int kernelSize = 1 + radius * 2;
	unsigned int* kernel = (unsigned int*)malloc(kernelSize * sizeof(unsigned int));
	memset(kernel, 0, kernelSize * sizeof(unsigned int));
	int(*mult)[256] = (int(*)[256])malloc(kernelSize * 256 * sizeof(int));
	memset(mult, 0, kernelSize * 256 * sizeof(int));

	int xStart = 0;
	int yStart = 0;
	w = xStart + w - glm::max(0, (xStart + w) - w);
	h = yStart + h - glm::max(0, (yStart + h) - h);
	int imageSize = w*h;
	int widthstep = w*channels;
	if (channels == 3 || channels == 4)
	{
		unsigned char *    CacheImg = nullptr;
		CacheImg = (unsigned char *)malloc(sizeof(unsigned char) * imageSize * 6);
		if (CacheImg == nullptr) return;
		unsigned char *    rCache = CacheImg;
		unsigned char *    gCache = CacheImg + imageSize;
		unsigned char *    bCache = CacheImg + imageSize * 2;
		unsigned char *    r2Cache = CacheImg + imageSize * 3;
		unsigned char *    g2Cache = CacheImg + imageSize * 4;
		unsigned char *    b2Cache = CacheImg + imageSize * 5;
		int sum = 0;
		for (int K = 1; K < radius; K++) {
			unsigned int szi = radius - K;
			kernel[radius + K] = kernel[szi] = szi*szi;
			sum += kernel[szi] + kernel[szi];
			for (int j = 0; j < 256; j++) {
				mult[radius + K][j] = mult[szi][j] = kernel[szi] * j;
			}
		}
		kernel[radius] = radius*radius;
		sum += kernel[radius];
		for (int j = 0; j < 256; j++) {
			mult[radius][j] = kernel[radius] * j;
		}
		for (int Y = 0; Y < h; ++Y) {
			unsigned char*     LinePS = img + Y*widthstep;
			unsigned char*     LinePR = rCache + Y*w;
			unsigned char*     LinePG = gCache + Y*w;
			unsigned char*     LinePB = bCache + Y*w;
			for (int X = 0; X < w; ++X) {
				int     p2 = X*channels;
				LinePR[X] = LinePS[p2];
				LinePG[X] = LinePS[p2 + 1];
				LinePB[X] = LinePS[p2 + 2];
			}
		}
		int kernelsum = 0;
		for (int K = 0; K < kernelSize; K++) {
			kernelsum += kernel[K];
		}
		float fkernelsum = 1.0f / kernelsum;
		for (int Y = yStart; Y < h; Y++) {
			int heightStep = Y * w;
			unsigned char*     LinePR = rCache + heightStep;
			unsigned char*     LinePG = gCache + heightStep;
			unsigned char*     LinePB = bCache + heightStep;
			for (int X = xStart; X < w; X++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned    int     readPos = ((X - radius + K + w) % w);
					int * pmult = mult[K];
					cr += pmult[LinePR[readPos]];
					cg += pmult[LinePG[readPos]];
					cb += pmult[LinePB[readPos]];
				}
				unsigned int p = heightStep + X;
				r2Cache[p] = cr* fkernelsum;
				g2Cache[p] = cg* fkernelsum;
				b2Cache[p] = cb* fkernelsum;
			}
		}
		for (int X = xStart; X < w; X++) {
			int WidthComp = X*channels;
			int WidthStep = w*channels;
			unsigned char*     LinePS = img + X*channels;
			unsigned char*     LinePR = r2Cache + X;
			unsigned char*     LinePG = g2Cache + X;
			unsigned char*     LinePB = b2Cache + X;
			for (int Y = yStart; Y < h; Y++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned int   readPos = ((Y - radius + K + h) % h) * w;
					int * pmult = mult[K];
					cr += pmult[LinePR[readPos]];
					cg += pmult[LinePG[readPos]];
					cb += pmult[LinePB[readPos]];
				}
				int    p = Y*WidthStep;
				LinePS[p] = (unsigned char)(cr * fkernelsum);
				LinePS[p + 1] = (unsigned char)(cg * fkernelsum);
				LinePS[p + 2] = (unsigned char)(cb* fkernelsum);


			}
		}
		free(CacheImg);
	}
	else if (channels == 1)
	{
		unsigned char *    CacheImg = nullptr;
		CacheImg = (unsigned char *)malloc(sizeof(unsigned char) * imageSize * 2);
		if (CacheImg == nullptr) return;
		unsigned char *    rCache = CacheImg;
		unsigned char *    r2Cache = CacheImg + imageSize;

		int sum = 0;
		for (int K = 1; K < radius; K++) {
			unsigned int szi = radius - K;
			kernel[radius + K] = kernel[szi] = szi*szi;
			sum += kernel[szi] + kernel[szi];
			for (int j = 0; j < 256; j++) {
				mult[radius + K][j] = mult[szi][j] = kernel[szi] * j;
			}
		}
		kernel[radius] = radius*radius;
		sum += kernel[radius];
		for (int j = 0; j < 256; j++) {
			mult[radius][j] = kernel[radius] * j;
		}
		for (int Y = 0; Y < h; ++Y) {
			unsigned char*     LinePS = img + Y*widthstep;
			unsigned char*     LinePR = rCache + Y*w;
			for (int X = 0; X < w; ++X) {
				LinePR[X] = LinePS[X];
			}
		}
		int kernelsum = 0;
		for (int K = 0; K < kernelSize; K++) {
			kernelsum += kernel[K];
		}
		float fkernelsum = 1.0f / kernelsum;
		for (int Y = yStart; Y < h; Y++) {
			int heightStep = Y * w;
			unsigned char*     LinePR = rCache + heightStep;
			for (int X = xStart; X < w; X++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned    int     readPos = ((X - radius + K + w) % w);
					int * pmult = mult[K];
					cr += pmult[LinePR[readPos]];
				}
				unsigned int p = heightStep + X;
				r2Cache[p] = cr * fkernelsum;
			}
		}
		for (int X = xStart; X < w; X++) {
			int WidthComp = X*channels;
			int WidthStep = w*channels;
			unsigned char*     LinePS = img + X*channels;
			unsigned char*     LinePR = r2Cache + X;
			for (int Y = yStart; Y < h; Y++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned int   readPos = ((Y - radius + K + h) % h) * w;
					int * pmult = mult[K];
					cr += pmult[LinePR[readPos]];
				}
				int    p = Y*WidthStep;
				LinePS[p] = (unsigned char)(cr* fkernelsum);
			}
		}
		free(CacheImg);
	}
	free(kernel);
	free(mult);
}

std::vector<float> boxesForGauss(int sigma, int n)  // standard deviation, number of boxes
{
	float wIdeal = sqrt((12 * sigma*sigma / n) + 2);  // Ideal averaging filter w 
	int wl = floor(wIdeal);
	if (wl % 2 == 0) wl--;
	int wu = wl + 2;

	float mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	int m = round(mIdeal);
	// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	std::vector<float> sizes;
	for (int i = 0; i < n; i++)
		sizes.push_back(i < m ? wl : wu);
	return sizes;
}

//standard gaussian
void gaussianBlur(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int sigma)
{
	int radius = ceil(sigma * 2.57);
	for (int imageRow = 0; imageRow < h; imageRow++)
	{
		for (int imageColWithChannel = 0; imageColWithChannel < w*ch; imageColWithChannel += ch)
		{
			glm::vec3 color = glm::vec3(0.0f);
			float allWeights = 0.0f;
			for (int kernelRow = imageRow - radius; kernelRow < imageRow + radius + 1; kernelRow++)
			{
				for (int kernelColWithChannel = imageColWithChannel - radius*ch; kernelColWithChannel < imageColWithChannel + (radius + 1)*ch; kernelColWithChannel += ch)
				{
					int dsq = (kernelColWithChannel / ch - imageColWithChannel / ch)*(kernelColWithChannel / ch - imageColWithChannel / ch) + (kernelRow - imageRow)*(kernelRow - imageRow);//x^2 + y^2
					float weight = exp(-dsq / (2 * sigma * sigma)) / (PI * 2 * sigma * sigma);//gaussian function: 1/(2*pi*sgima^2) * e^(-(x^2+y^2)/(2*sigma^2))

					int x = glm::min((w - 1) * ch, glm::max(0, kernelColWithChannel));
					int y = glm::min(h - 1, glm::max(0, kernelRow));

					color.r += scl[y*w*ch + x] * weight;
					color.g += scl[y*w*ch + x + 1] * weight;
					color.b += scl[y*w*ch + x + 2] * weight;

					allWeights += weight;
				}
			}
			tcl[imageRow*w*ch + imageColWithChannel] = glm::round(color.r / allWeights);
			tcl[imageRow*w*ch + imageColWithChannel + 1] = glm::round(color.g / allWeights);
			tcl[imageRow*w*ch + imageColWithChannel + 2] = glm::round(color.b / allWeights);
		}
	}
}

//algorithm2
void boxBlur_2(unsigned char *scl, int w, int h, int ch, int r)
{
	for (int imageRow = 0; imageRow < h; imageRow++)
	{
		for (int imageColWithChannel = 0; imageColWithChannel < w*ch; imageColWithChannel += ch)
		{
			glm::vec3 color = glm::vec3(0.0f);
			for (int kernelRow = imageRow - r; kernelRow < imageRow + r + 1; kernelRow++)
			{
				for (int kernelColWithChannel = imageColWithChannel - r*ch; kernelColWithChannel < imageColWithChannel + (r + 1)*ch; kernelColWithChannel += ch)
				{
					int x = glm::min((w - 1) * ch, glm::max(0, kernelColWithChannel));
					int y = glm::min(h - 1, glm::max(0, kernelRow));

					color.r += scl[y*w*ch + x];
					color.g += scl[y*w*ch + x + 1];
					color.b += scl[y*w*ch + x + 2];
				}
			}
			scl[imageRow*w*ch + imageColWithChannel] = color.r / ((r + r + 1)*(r + r + 1));
			scl[imageRow*w*ch + imageColWithChannel + 1] = color.g / ((r + r + 1)*(r + r + 1));
			scl[imageRow*w*ch + imageColWithChannel + 2] = color.b / ((r + r + 1)*(r + r + 1));
		}
	}
}

void gaussianBlur2(unsigned char *scl, int w, int h, int ch, int r) {
	std::vector<float> bxs = boxesForGauss(r, 3);
	boxBlur_2(scl, w, h, ch, (bxs[0] - 1) / 2);
	boxBlur_2(scl, w, h, ch, (bxs[1] - 1) / 2);
	boxBlur_2(scl, w, h, ch, (bxs[2] - 1) / 2);
}

//algorithm3
void boxBlurH_3(unsigned char *scl, int w, int h, int ch, int r)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w * ch; j += ch)
		{
			glm::vec3 color = glm::vec3(0.0f);
			for (int ix = j - r*ch; ix < j + r*ch + ch; ix += ch)
			{
				int x = glm::min(w * ch - ch, glm::max(0, ix));
				color.r += scl[i*w*ch + x];
				color.g += scl[i*w*ch + x + 1];
				color.b += scl[i*w*ch + x + 2];
			}
			scl[i*w*ch + j] = color.r / (r + r + 1);
			scl[i*w*ch + j + 1] = color.g / (r + r + 1);
			scl[i*w*ch + j + 2] = color.b / (r + r + 1);
		}
}

void boxBlurT_3(unsigned char *scl, int w, int h, int ch, int r)
{
	for (int i = 0; i < h; i++)
		for (int j = 0; j < w*ch; j += ch)
		{
			glm::vec3 color = glm::vec3(0.0f);
			for (int iy = i - r; iy < i + r + 1; iy++)
			{
				int y = glm::min(h - 1, glm::max(0, iy));
				color.r += scl[y*w*ch + j];
				color.g += scl[y*w*ch + j + 1];
				color.b += scl[y*w*ch + j + 2];
			}
			scl[i*w*ch + j] = color.r / (r + r + 1);
			scl[i*w*ch + j + 1] = color.g / (r + r + 1);
			scl[i*w*ch + j + 2] = color.b / (r + r + 1);
		}
}

void boxBlur_3(unsigned char *scl, int w, int h, int ch, int r)
{
	boxBlurH_3(scl, w, h, ch, r);
	boxBlurT_3(scl, w, h, ch, r);
}

void gaussianBlur3(unsigned char *scl, int w, int h, int ch, int r) {
	std::vector<float> bxs = boxesForGauss(r, 3);
	boxBlur_3(scl, w, h, ch, (bxs[0] - 1) / 2);
	boxBlur_3(scl, w, h, ch, (bxs[1] - 1) / 2);
	boxBlur_3(scl, w, h, ch, (bxs[2] - 1) / 2);
}

//algorithm4
void boxBlurH_4(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	float iarr = 1.0f / (r + r + 1.0f);
	for (int i = 0; i < h; i++) {
		int ti = i*w*ch;//middle
		int li = ti;//left index
		int ri = ti + r*ch;//right index
		glm::vec3 fv = glm::vec3(scl[ti], scl[ti + 1], scl[ti + 2]);//first value
		glm::vec3 lv = glm::vec3(scl[ti + (w - 1)*ch], scl[ti + (w - 1)*ch + 1], scl[ti + (w - 1)*ch + 2]);//last value
		glm::vec3 val = glm::vec3(fv.r*(r + 1), fv.g*(r + 1), fv.b*(r + 1));//(r+1)/(2r+1)
		for (int j = 0; j < r*ch; j += ch)
		{
			val.r += scl[ti + j];
			val.g += scl[ti + j + 1];
			val.b += scl[ti + j + 2];
		}
		for (int j = 0; j <= r*ch; j += ch)
		{
			val.r += scl[ri] - fv.r;
			val.g += scl[ri + 1] - fv.g;
			val.b += scl[ri + 2] - fv.b;

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

			ri += ch;
			ti += ch;
		}
		for (int j = (r + 1)*ch; j < (w - r)*ch; j += ch)
		{
			val.r += scl[ri] - scl[li];
			val.g += scl[ri + 1] - scl[li + 1];
			val.b += scl[ri + 2] - scl[li + 2];

			tcl[ti] = glm::round(val.r*iarr);
			tcl[ti + 1] = glm::round(val.g*iarr);
			tcl[ti + 2] = glm::round(val.b*iarr);

			ri += ch;
			li += ch;
			ti += ch;
		}
		for (int j = (w - r)*ch; j < w*ch; j += ch)
		{
			val.r += lv.r - scl[li];
			val.g += lv.g - scl[li + 1];
			val.b += lv.b - scl[li + 2];

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

			li += ch;
			ti += ch;
		}
	}
}

void boxBlurT_4(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	float iarr = 1.0f / (r + r + 1.0f);
	for (int i = 0; i < w*ch; i += ch) {
		int ti = i;
		int li = ti;
		int ri = ti + r*w*ch;
		glm::vec3 fv = glm::vec3(scl[ti], scl[ti + 1], scl[ti + 2]);
		glm::vec3 lv = glm::vec3(scl[ti + w*(h - 1)*ch], scl[ti + w*(h - 1)*ch + 1], scl[ti + w*(h - 1)*ch + 2]);
		glm::vec3 val = glm::vec3((r + 1)*fv.r, (r + 1)*fv.g, (r + 1)*fv.b);
		for (int j = 0; j < r; j++)
		{
			val.r += scl[ti + j*w*ch];
			val.g += scl[ti + j*w*ch + 1];
			val.b += scl[ti + j*w*ch + 2];
		}
		for (int j = 0; j <= r; j++)
		{
			val.r += scl[ri] - fv.r;
			val.g += scl[ri + 1] - fv.g;
			val.b += scl[ri + 2] - fv.b;

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

			ri += w*ch;
			ti += w*ch;
		}
		for (int j = r + 1; j < h - r; j++)
		{
			val.r += scl[ri] - scl[li];
			val.g += scl[ri + 1] - scl[li + 1];
			val.b += scl[ri + 2] - scl[li + 2];

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

			li += w*ch;
			ri += w*ch;
			ti += w*ch;
		}
		for (int j = h - r; j < h; j++)
		{
			val.r += lv.r - scl[li];
			val.g += lv.g - scl[li + 1];
			val.b += lv.b - scl[li + 2];

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

			li += w*ch;
			ti += w*ch;
		}
	}
}

void boxBlur_4(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	memcpy(tcl, scl, w*h*ch);
	boxBlurH_4(tcl, scl, w, h, ch, r);
	boxBlurT_4(scl, tcl, w, h, ch, r);
}

void gaussianBlur4(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	std::vector<float> bxs = boxesForGauss(r, 3);
	boxBlur_4(scl, tcl, w, h, ch, (bxs[0] - 1) / 2);
	boxBlur_4(tcl, scl, w, h, ch, (bxs[1] - 1) / 2);
	boxBlur_4(scl, tcl, w, h, ch, (bxs[2] - 1) / 2);
}

//callback
void framebuffer_size_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}
void process_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

//render
void initGL()
{
	glViewport(0, 0, windowWidth, windowHeight);
	shaderProgram = initShaderProgram(bigTReadFileUtil.readFile("./simple_vertex_shader.vs"), bigTReadFileUtil.readFile("./simple_fragment_shader.fs"));
	initTriangle();
	initTexture();
}

void initTexture()
{
	glGenTextures(1, &triangleTex);
	glBindTexture(GL_TEXTURE_2D, triangleTex);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	int w, h, ch;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("D:/Resource/desktop/192x120.jpg", &w, &h, &ch, 0);
	//0x08729ba0
	if (data)
	{
		GLint format = 0;
		if (ch == 4)
		{
			format = GL_RGBA;
		}
		if (ch == 3)
		{
			format = GL_RGB;
		}
		//down sample
		FILETIME ft;
		timer_start(&ft);
		int radius = 10;
		//800x200, radius=5, time=18660 blur,calc weight
		//800x200, radius=5, time=12059 blur,no calc weight
		//800x200, radius=5, time=5343 blur2
		//800x200, radius=5, time=592 blur3
		//800x200, radius=5, time=102 blur4
		unsigned char *new_data = new unsigned char[w * h * ch];
		gaussianBlur4(data, new_data, w, h, ch, radius);
		//GaussianBlur(data, w, h, ch, radius);
		int time = timer_elapsed_msec(&ft);
		printf("%dx%d, radius=%d, time=%d\n", w, h, radius, time);
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		delete[] new_data;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource)
{
	bigTShaderUtil.isEnableDebug = GL_TRUE;
	GLuint vertexShader = bigTShaderUtil.initShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragShader = bigTShaderUtil.initShader(fragShaderSource, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
#if SHADER_DEBUG
	int  success;
	char infoLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("LINK PROGRAM ERROR %s\n", std::string(infoLog).c_str());
	}
	else
	{
		printf("LINK PROGRAM SUCCESS\n");
	}
#endif
	glUseProgram(program);
	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
	return program;
}

void initTriangle()
{
	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	static float sigmaSquare = 10.0f;
	sigmaSquare += 0.03f;
	//printf("sigmaSquare = %f\n", sigmaSquare);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glUniform1f(glGetUniformLocation(shaderProgram, "gaussian_width"), 960.0f);
	glUniform1f(glGetUniformLocation(shaderProgram, "gaussian_height"), 600.0f);
	//glUniform1f(glGetUniformLocation(shaderProgram, "sigmaSquarea"), 15.0f);//15.609950
	//glUniform1i(glGetUniformLocation(shaderProgram, "gaussian_radius"), 7);
	glBindVertexArray(triangleVAO);
	glBindTexture(GL_TEXTURE_2D, triangleTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
}