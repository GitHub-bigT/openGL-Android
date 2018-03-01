#include <iostream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//inner
#include "ShaderUtil.h"
#include "ReadFileUtil.h"

//const
const int windowWidth = 800;
const int windowHeight = 600;
//common property
GLuint shaderProgram;

//common Util
#define SHADER_DEBUG 1;
ShaderUtil bigTShaderUtil;
TextFile bigTReadFileUtil;

//Triangle
#define TRIANGLE_DEBUG 0;
GLuint triangleVAO;
GLuint triangleVBO;
GLfloat triangleVertices[] =
{
	-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // 左上
	0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // 右上
	0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // 右下
	-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // 左下
};

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

//render
void initGL();
GLuint initShaderProgram(const char* vertexShaderSource, const char* geometryShaderSource, const char* fragShaderSource);
void initTriangle();
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
	shaderProgram = initShaderProgram(bigTReadFileUtil.readFile("./simple_vertex_shader.vs"), bigTReadFileUtil.readFile("./simple_geometry_shader.gs"), bigTReadFileUtil.readFile("./simple_fragment_shader.fs"));
	initTriangle();
}

GLuint initShaderProgram(const char* vertexShaderSource, const char* geometryShaderSource, const char* fragShaderSource)
{
	bigTShaderUtil.isEnableDebug = GL_TRUE;
	GLuint vertexShader = bigTShaderUtil.initShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint geometryShader = bigTShaderUtil.initShader(geometryShaderSource, GL_GEOMETRY_SHADER);
	GLuint fragShader = bigTShaderUtil.initShader(fragShaderSource, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, geometryShader);
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
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
	glBindVertexArray(triangleVAO);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	glDrawArrays(GL_POINTS, 0, 4);

	glfwSwapBuffers(window);
}