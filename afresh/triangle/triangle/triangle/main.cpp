#include <iostream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//inner
#include "ShaderUtil.h"
#include "ReadFileUtil.h"

//const
const int windowWidth	= 800;
const int windowHeight	= 600;
//common property


//common Util
#define SHADER_DEBUG 1;
ShaderUtil bigTShaderUtil;
TextFile bigTReadFileUtil;

//Triangle
#define TRIANGLE_DEBUG 1;
GLuint triangleVBO;
GLfloat triangleVertices[] =
{
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	0.0f,  0.5f, 0.0f
};

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

//render
void initGL();
void initTriangle();
void draw_scene(GLFWwindow *window);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

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

	//gl
	glViewport(0, 0, windowWidth, windowHeight);
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
	initTriangle();
	bigTShaderUtil.isEnableDebug = GL_TRUE;
	GLuint vertexShader = bigTShaderUtil.initShader(bigTReadFileUtil.readFile("./simple_vertex_shader.vs"), GL_VERTEX_SHADER);
	GLuint fragShader = bigTShaderUtil.initShader(bigTReadFileUtil.readFile("./simple_fragment_shader.fs"), GL_FRAGMENT_SHADER);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(vertexShader, GL_VERTEX_SHADER);
	glAttachShader(vertexShader, GL_FRAGMENT_SHADER);
	glLinkProgram(shaderProgram);
#if SHADER_DEBUG
	int  success;
	char infoLog[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		printf("%s\n",std::string(infoLog).c_str());
	}
#endif
	glUseProgram(shaderProgram);
	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);
}

void initTriangle()
{
	GLuint triangleVAO;
	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);

	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVBO), triangleVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
}

void draw_scene(GLFWwindow *window)
{
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glfwSwapBuffers(window);
}