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

struct FBO
{
	GLuint fbo;
	GLuint tex;
	GLuint rbo;
	int width;
	int height;
	FBO()
	{
		fbo = 0;
		tex = 0;
		rbo = 0;
		width = 0;
		height = 0;
	}
};

//const
const int windowWidth = 960;
const int windowHeight = 600;
int multiple = 1;
float PI = 3.141592653589793;
//common property
GLuint sDownSampleShader;
GLuint sStandardGaussianShader;
GLuint sSimpleShader;
GLuint sBoxShader;
GLuint sBoxHShader;
GLuint sBoxVShader;
GLfloat radius = 0.0f;

//common Util
#define SHADER_DEBUG 1;
ShaderUtil bigTShaderUtil;
TextFile bigTReadFileUtil;

//Triangle
FBO gaussianFbos[3];
GLuint triangleVAO;
GLuint triangleVBO;
GLuint rawTex;
GLuint processTex;
FBO downSampleFbo;
FBO gaussianFbo;
int rawImageWidth, rawImageHeight, rawImageChannels;
int processImageWidth, processImageHeight, processImageChannels;

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
FBO getFbo(int w, int h);
void dowmSample(unsigned char *scl, unsigned char *tcl, int mul);
void deleteFbo(FBO &fbo);
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

	if (glIsVertexArray(triangleVAO))
	{
		glDeleteVertexArrays(1, &triangleVAO);
		triangleVAO = 0;
	}

	if (glIsBuffer(triangleVBO))
	{
		glDeleteBuffers(1, &triangleVBO);
		triangleVBO = 0;
	}

	if (glIsTexture(rawTex))
	{
		glDeleteTextures(1, &rawTex);
		rawTex = 0;
	}

	for (int i = 0; i < sizeof(gaussianFbos) / sizeof(FBO); i++)
	{
		deleteFbo(gaussianFbos[i]);
	}

	deleteFbo(gaussianFbo);

	deleteFbo(downSampleFbo);

	//release glfw
	glfwTerminate();

	return 0;
}

void check_error()
{
	int err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("GL error 0x%x\n", err);
	}
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

std::vector<int> boxesForGauss(int sigma, int n)  // standard deviation, number of boxes
{
	float wIdeal = sqrt((12 * sigma*sigma / n) + 2);  // Ideal averaging filter w 
	int wl = floor(wIdeal);
	if (wl % 2 == 0) wl--;
	int wu = wl + 2;

	float mIdeal = (12 * sigma*sigma - n*wl*wl - 4 * n*wl - 3 * n) / (-4 * wl - 4);
	int m = round(mIdeal);
	// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

	std::vector<int> sizes;
	for (int i = 0; i < n; i++)
		sizes.push_back(i < m ? wl : wu);
	return sizes;
}

//standard gaussian
void gaussianBlur(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int sigma)
{
	int radius = ceil(sigma * 2.57);
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w*ch; j += ch)
		{
			glm::vec3 color = glm::vec3(0.0f);
			float allWeights = 0.0f;
			for (int ix = i - radius; ix < i + radius + 1; ix++)
			{
				for (int iy = j - radius*ch; iy < j + (radius + 1)*ch; iy += ch)
				{
					int dsq = (iy / ch - j / ch)*(iy / ch - j / ch) + (ix - i)*(ix - i);//x^2 + y^2
					float weight = exp(-dsq / (2 * sigma * sigma)) / (PI * 2 * sigma * sigma);//gaussian function: 1/(2*pi*sgima^2) * e^(-(x^2+y^2)/(2*sigma^2))

					int x = glm::min((w - 1) * ch, glm::max(0, iy));
					int y = glm::min(h - 1, glm::max(0, ix));

					color.r += scl[y*w*ch + x] * weight;
					color.g += scl[y*w*ch + x + 1] * weight;
					color.b += scl[y*w*ch + x + 2] * weight;

					allWeights += weight;
				}
			}
			tcl[i*w*ch + j] = glm::round(color.r / allWeights);
			tcl[i*w*ch + j + 1] = glm::round(color.g / allWeights);
			tcl[i*w*ch + j + 2] = glm::round(color.b / allWeights);
		}
	}
}

//algorithm2
void boxBlur_2(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w*ch; j += ch)
		{
			glm::vec3 val = glm::vec3(0.0f);
			for (int ix = i - r; ix < i + r + 1; ix++)
			{
				for (int iy = j - r*ch; iy < j + (r + 1)*ch; iy += ch)
				{
					int x = glm::min((w - 1) * ch, glm::max(0, iy));
					int y = glm::min(h - 1, glm::max(0, ix));

					val.r += scl[y*w*ch + x];
					val.g += scl[y*w*ch + x + 1];
					val.b += scl[y*w*ch + x + 2];
				}
			}
			tcl[i*w*ch + j] = val.r / ((r + r + 1)*(r + r + 1));
			tcl[i*w*ch + j + 1] = val.g / ((r + r + 1)*(r + r + 1));
			tcl[i*w*ch + j + 2] = val.b / ((r + r + 1)*(r + r + 1));
		}
	}
}

void gaussianBlur2(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r) {
	std::vector<int> bxs = boxesForGauss(r, 3);
	boxBlur_2(scl, tcl, w, h, ch, (bxs[0] - 1) / 2);
	boxBlur_2(tcl, scl, w, h, ch, (bxs[1] - 1) / 2);
	boxBlur_2(scl, tcl, w, h, ch, (bxs[2] - 1) / 2);
}

//algorithm3
void boxBlurH_3(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
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
			tcl[i*w*ch + j] = color.r / (r + r + 1);
			tcl[i*w*ch + j + 1] = color.g / (r + r + 1);
			tcl[i*w*ch + j + 2] = color.b / (r + r + 1);
		}
}

void boxBlurT_3(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
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
			tcl[i*w*ch + j] = color.r / (r + r + 1);
			tcl[i*w*ch + j + 1] = color.g / (r + r + 1);
			tcl[i*w*ch + j + 2] = color.b / (r + r + 1);
		}
}

void boxBlur_3(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	memcpy(tcl, scl, w*h*ch);
	boxBlurH_3(tcl, scl, w, h, ch, r);
	boxBlurT_3(scl, tcl, w, h, ch, r);
}

void gaussianBlur3(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r) {
	std::vector<int> bxs = boxesForGauss(r, 3);
	boxBlur_3(scl, tcl, w, h, ch, (bxs[0] - 1) / 2);
	boxBlur_3(tcl, scl, w, h, ch, (bxs[1] - 1) / 2);
	boxBlur_3(scl, tcl, w, h, ch, (bxs[2] - 1) / 2);
}

//algorithm4
void boxBlurH_4(unsigned char *scl, unsigned char *tcl, int w, int h, int ch, int r)
{
	float iarr = 1.0f / (r + r + 1.0f);
	for (int i = 0; i < h; i++) {
		int ti = i*w*ch;//middle index
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

			tcl[ti] = val.r*iarr;
			tcl[ti + 1] = val.g*iarr;
			tcl[ti + 2] = val.b*iarr;

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
	std::vector<int> bxs = boxesForGauss(r, 3);
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
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		multiple += 1;
		printf("multiple=%d\n", multiple);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		multiple -= 1;
		if (multiple < 1)
		{
			multiple = 1;
		}
		printf("multiple=%d\n", multiple);
	}
}

//render
void initGL()
{
	sDownSampleShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./DownSample.fs"));
	sStandardGaussianShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./StandardGaussian.fs"));
	sSimpleShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./Simple.fs"));
	//sBoxShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./BoxGaussianBlur.fs"));
	//sBoxHShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./BoxGaussianBlurH.fs"));
	//sBoxVShader = initShaderProgram(bigTReadFileUtil.readFile("./Simple.vs"), bigTReadFileUtil.readFile("./BoxGaussianBlurV.fs"));
	initTriangle();
	initTexture();

	for (int i = 0; i < sizeof(gaussianFbos) / sizeof(FBO); i++)
	{
		gaussianFbos[i] = getFbo(rawImageWidth / multiple, rawImageHeight / multiple);
	}
}

FBO getFbo(int w, int h)
{
	FBO fbo;
	fbo.width = w;
	fbo.height = h;
	glGenFramebuffers(1, &fbo.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fbo);
	glGenTextures(1, &fbo.tex);
	glBindTexture(GL_TEXTURE_2D, fbo.tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.tex, 0);

/*
	glGenRenderbuffers(1, &fbo.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, fbo.rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;*/
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return fbo;
}

void deleteFbo(FBO &fbo)
{
	if (glIsTexture(fbo.tex))
	{
		glDeleteTextures(1, &fbo.tex);
		fbo.tex = 0;
	}
	if (glIsRenderbuffer(fbo.rbo))
	{
		glDeleteRenderbuffers(1, &fbo.rbo);
		fbo.rbo = 0;
	}
	if (glIsFramebuffer(fbo.fbo))
	{
		glDeleteFramebuffers(1, &fbo.fbo);
		fbo.fbo = 0;
	}
}

void dowmSample(unsigned char *scl, unsigned char *tcl, int mul)
{
	for (int i = 0; i < processImageHeight; i++)
	{
		for (int j = 0; j < processImageWidth * processImageChannels; j += processImageChannels)
		{
			tcl[i*processImageWidth*processImageChannels + j] = scl[mul*i*rawImageWidth*rawImageChannels + mul*j];
			tcl[i*processImageWidth*processImageChannels + j + 1] = scl[mul*i*rawImageWidth*rawImageChannels + mul*j + 1];
			tcl[i*processImageWidth*processImageChannels + j + 2] = scl[mul*i*rawImageWidth*rawImageChannels + mul*j + 2];
		}
	}
}

void initTexture()
{
	glGenTextures(1, &rawTex);
	glBindTexture(GL_TEXTURE_2D, rawTex);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("D:/Resource/desktop/1920x1200.jpg", &rawImageWidth, &rawImageHeight, &rawImageChannels, 0);
	//processImageWidth = rawImageWidth / multiple;
	//processImageHeight = rawImageHeight / multiple;
	//processImageChannels = rawImageChannels;
	//unsigned char *process_data = new unsigned char[processImageWidth * processImageHeight * processImageChannels];
	//FILETIME ft;
	//timer_start(&ft);
	//dowmSample(data, process_data, multiple);

	//0x08729ba0
	if (data)
	{
		GLint format = 0;
		if (rawImageChannels == 4)
			format = GL_RGBA;
		if (rawImageChannels == 3)
			format = GL_RGB;
		//down sample
		//FILETIME ft;
		//timer_start(&ft);
		//int radius = 5;
		//800x200, radius=5, time=17778 blur,calc weight
		//800x200, radius=5, time=12059 blur,no calc weight
		//800x200, radius=5, time=5002 blur2
		//800x200, radius=5, time=592 blur3
		//800x200, radius=5, time=12 blur4
		//240x150, radius=5, downsample + gaussian time=2~4 blur4
		//unsigned char *new_data = new unsigned char[processImageWidth * processImageHeight * processImageChannels];
		//gaussianBlur4(process_data, new_data, processImageWidth, processImageHeight, processImageChannels, 5.0f);
		//int time = timer_elapsed_msec(&ft);
		//printf("%dx%d, radius=%d, time=%d\n", rawImageWidth, rawImageHeight, radius, time);
		//glTexImage2D(GL_TEXTURE_2D, 0, format, processImageWidth, processImageHeight, 0, format, GL_UNSIGNED_BYTE, new_data);
		//int downsampleGauTime = timer_elapsed_msec(&ft);
		//printf("downsample + gaussian time = %d, width: %d -> %d, height: %d -> %d\n", downsampleGauTime, rawImageWidth, processImageWidth, rawImageHeight, processImageHeight);
		//delete[] process_data;
		glTexImage2D(GL_TEXTURE_2D, 0, format, rawImageWidth, rawImageHeight, 0, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
		//delete[] new_data;
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

void dump_texture(GLuint texture_id, int *pWidth, int *pHeight)
{
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, pWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, pHeight);
	uint8_t *pix = (uint8_t*)malloc(*pWidth * *pHeight * 4);
	char buff[128];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);
	sprintf(buff, "dump_%dx%d.rgba", *pWidth, *pHeight);
	FILE *fp = fopen(buff, "wb");
	fwrite(pix, 1, *pWidth * *pHeight * 4, fp);
	fclose(fp);
	free(pix);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_scene(GLFWwindow *window)
{
	//GLint maxAttach = 0;
	//glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttach);
	FILETIME ft;
	timer_start(&ft);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(triangleVAO);

	if (multiple > 1)//radius > 0.05
	{
		deleteFbo(downSampleFbo);
		downSampleFbo = getFbo(rawImageWidth / multiple, rawImageHeight / multiple);

		deleteFbo(gaussianFbo);
		gaussianFbo = getFbo(rawImageWidth / multiple, rawImageHeight / multiple);

		//pass 1
		//down sample + gaussian blur
		glUseProgram(sDownSampleShader);

		glViewport(0, 0, downSampleFbo.width, downSampleFbo.height);
		glBindFramebuffer(GL_FRAMEBUFFER, downSampleFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, rawTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		processTex = downSampleFbo.tex;

		//gaussian, algorithm2
	/*
		glUseProgram(sBoxShader);
		std::vector<int> bxs = boxesForGauss(40.8f, 3);
		for (int i = 0; i < bxs.size(); i++)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, processFbos[(i+1)%2].fbo);
			glBindTexture(GL_TEXTURE_2D, processFbos[i % 2].tex);
			glUniform1i(glGetUniformLocation(sBoxShader, "gaussian_width"), windowWidth);
			glUniform1i(glGetUniformLocation(sBoxShader, "gaussian_height"), windowHeight);
			glUniform1i(glGetUniformLocation(sBoxShader, "r"), (bxs[i] - 1) / 2);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		glBindTexture(GL_TEXTURE_2D, 0);*/

		//gaussian, algorithm3W
	/*
		std::vector<int> bxs = boxesForGauss(radius, 3);
		for (int i = 0; i < bxs.size(); i++)
		{
			//H
			glBindFramebuffer(GL_FRAMEBUFFER, gaussianFbos[1].fbo);
			if (i == 0)
				glBindTexture(GL_TEXTURE_2D, processTex);
			else
				glBindTexture(GL_TEXTURE_2D, gaussianFbos[2].tex);
			glUseProgram(sBoxHShader);
			glUniform1i(glGetUniformLocation(sBoxHShader, "gaussian_width"), windowWidth);
			glUniform1i(glGetUniformLocation(sBoxHShader, "gaussian_height"), windowHeight);
			glUniform1i(glGetUniformLocation(sBoxHShader, "r"), (bxs[i] - 1) / 2);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			//V
			glBindFramebuffer(GL_FRAMEBUFFER, gaussianFbos[2].fbo);
			glBindTexture(GL_TEXTURE_2D, gaussianFbos[1].tex);
			glUseProgram(sBoxVShader);
			glUniform1i(glGetUniformLocation(sBoxVShader, "gaussian_width"), windowWidth);
			glUniform1i(glGetUniformLocation(sBoxVShader, "gaussian_height"), windowHeight);
			glUniform1i(glGetUniformLocation(sBoxVShader, "r"), (bxs[i] - 1) / 2);
			glDrawArrays(GL_TRIANGLES, 0, 6);
		}
		processTex = gaussianFbos[2].tex;
		glBindTexture(GL_TEXTURE_2D, 0);*/

		//standard gaussian
		glUseProgram(sStandardGaussianShader);
		glUniform1f(glGetUniformLocation(sStandardGaussianShader, "gaussian_width"), gaussianFbo.width);
		glUniform1f(glGetUniformLocation(sStandardGaussianShader, "gaussian_height"), gaussianFbo.height);
		glViewport(0, 0, gaussianFbo.width, gaussianFbo.height);
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFbo.fbo);
		glBindTexture(GL_TEXTURE_2D, processTex);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		processTex = gaussianFbo.tex;


	}
	else
	{
		processTex = rawTex;
	}

	//pass 2
	glViewport(0, 0, windowWidth, windowHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(sSimpleShader);
	glBindTexture(GL_TEXTURE_2D, processTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	int renderTime = timer_elapsed_msec(&ft);
	//printf("renderTime = %d\n", renderTime);

	glfwSwapBuffers(window);
}