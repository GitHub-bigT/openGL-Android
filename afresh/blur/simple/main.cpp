#include <iostream>
#include <string>
#include <math.h>
#include <windows.h>

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
const int windowWidth = 800;
const int windowHeight = 200;
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
	- 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f
};

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

//render
void initGL();
GLuint initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource);
void initTriangle();
void initTexture();
void gaussianBlur(unsigned char *scl, int width, int height, int nrChannels, int gaussian_radius);
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

void GaussianBlur(unsigned char* img, int width, int height, int channels, int radius)
{
	radius = glm::min(glm::max(1, radius), 248);
	unsigned int kernelSize = 1 + radius * 2;
	unsigned int* kernel = (unsigned int*)malloc(kernelSize * sizeof(unsigned int));
	memset(kernel, 0, kernelSize * sizeof(unsigned int));
	int(*mult)[256] = (int(*)[256])malloc(kernelSize * 256 * sizeof(int));
	memset(mult, 0, kernelSize * 256 * sizeof(int));

	int xStart = 0;
	int yStart = 0;
	width = xStart + width - glm::max(0, (xStart + width) - width);
	height = yStart + height - glm::max(0, (yStart + height) - height);
	int imageSize = width*height;
	int widthstep = width*channels;
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
		for (int Y = 0; Y < height; ++Y) {
			unsigned char*     LinePS = img + Y*widthstep;
			unsigned char*     LinePR = rCache + Y*width;
			unsigned char*     LinePG = gCache + Y*width;
			unsigned char*     LinePB = bCache + Y*width;
			for (int X = 0; X < width; ++X) {
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
		for (int Y = yStart; Y < height; Y++) {
			int heightStep = Y * width;
			unsigned char*     LinePR = rCache + heightStep;
			unsigned char*     LinePG = gCache + heightStep;
			unsigned char*     LinePB = bCache + heightStep;
			for (int X = xStart; X < width; X++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned    int     readPos = ((X - radius + K + width) % width);
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
		for (int X = xStart; X < width; X++) {
			int WidthComp = X*channels;
			int WidthStep = width*channels;
			unsigned char*     LinePS = img + X*channels;
			unsigned char*     LinePR = r2Cache + X;
			unsigned char*     LinePG = g2Cache + X;
			unsigned char*     LinePB = b2Cache + X;
			for (int Y = yStart; Y < height; Y++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned int   readPos = ((Y - radius + K + height) % height) * width;
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
		for (int Y = 0; Y < height; ++Y) {
			unsigned char*     LinePS = img + Y*widthstep;
			unsigned char*     LinePR = rCache + Y*width;
			for (int X = 0; X < width; ++X) {
				LinePR[X] = LinePS[X];
			}
		}
		int kernelsum = 0;
		for (int K = 0; K < kernelSize; K++) {
			kernelsum += kernel[K];
		}
		float fkernelsum = 1.0f / kernelsum;
		for (int Y = yStart; Y < height; Y++) {
			int heightStep = Y * width;
			unsigned char*     LinePR = rCache + heightStep;
			for (int X = xStart; X < width; X++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned    int     readPos = ((X - radius + K + width) % width);
					int * pmult = mult[K];
					cr += pmult[LinePR[readPos]];
				}
				unsigned int p = heightStep + X;
				r2Cache[p] = cr * fkernelsum;
			}
		}
		for (int X = xStart; X < width; X++) {
			int WidthComp = X*channels;
			int WidthStep = width*channels;
			unsigned char*     LinePS = img + X*channels;
			unsigned char*     LinePR = r2Cache + X;
			for (int Y = yStart; Y < height; Y++) {
				int cb = 0;
				int cg = 0;
				int cr = 0;
				for (int K = 0; K < kernelSize; K++) {
					unsigned int   readPos = ((Y - radius + K + height) % height) * width;
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

//standard gaussian
void gaussianBlur(unsigned char *scl, int width, int height, int nrChannels, int sigma)
{
	int radius = ceil(sigma * 2.57);
	for (int imageRow = 0; imageRow < height; imageRow++)
	{
		for (int imageColWithChannel = 0; imageColWithChannel < width*nrChannels; imageColWithChannel+=nrChannels)
		{
			glm::vec3 color = glm::vec3(0.0f);
			float allWeights = 0.0f;
			for (int kernelRow = imageRow-radius; kernelRow < imageRow+radius+1; kernelRow++)
			{
				for (int kernelColWithChannel = imageColWithChannel-radius*nrChannels; kernelColWithChannel < imageColWithChannel+(radius+1)*nrChannels; kernelColWithChannel+=nrChannels)
				{			
					int dsq = (kernelColWithChannel/nrChannels - imageColWithChannel/nrChannels)*(kernelColWithChannel/nrChannels - imageColWithChannel/nrChannels) + (kernelRow - imageRow)*(kernelRow - imageRow);//x^2 + y^2
					float weight = exp(-dsq / (2 * sigma * sigma)) / (PI * 2 * sigma * sigma);//gaussian function: 1/(2*pi*sgima^2) * e^(-(x^2+y^2)/(2*sigma^2))
					
					int x = glm::min((width - 1) * nrChannels, glm::max(0, kernelColWithChannel));
					int y = glm::min(height - 1, glm::max(0, kernelRow));
					
					color.r += scl[y*width*nrChannels + x] * weight;
					color.g += scl[y*width*nrChannels + x +1] * weight;
					color.b += scl[y*width*nrChannels + x +2] * weight;

					allWeights += weight;
				}
			}
			scl[imageRow*width*nrChannels + imageColWithChannel] = glm::round(color.r / allWeights);
			scl[imageRow*width*nrChannels + imageColWithChannel + 1] = glm::round(color.g / allWeights);
			scl[imageRow*width*nrChannels + imageColWithChannel + 2] = glm::round(color.b / allWeights);
		}
	}
}

//callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char *data = stbi_load("D:/Resource/desktop/800x200.png", &width, &height, &nrChannels, 0);
	//0x08729ba0
	if (data)
	{
		GLint format = 0;
		if (nrChannels == 4)
		{
			format = GL_RGBA;
		}
		if (nrChannels == 3)
		{
			format = GL_RGB;
		}
		//down sample
		FILETIME ft;
		timer_start(&ft);
		int radius = 5;
		//800x200, radius=5, time=18660
		gaussianBlur(data, width, height, nrChannels, radius);
		//GaussianBlur(data, width, height, nrChannels, radius);
		int time = timer_elapsed_msec(&ft);
		printf("%dx%d, radius=%d, time=%d\n", width, height, radius, time);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
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