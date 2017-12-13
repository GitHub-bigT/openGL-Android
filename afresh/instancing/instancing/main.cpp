#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
//inner
#include "ShaderUtil.h"
#include "ReadFileUtil.h"

//const
const int windowWidth = 800;
const int windowHeight = 600;
const int instanceCount = 100;
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
GLfloat triangleVertices[] =
{
	// Î»ÖÃ          // ÑÕÉ«
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	0.05f,  0.05f,  0.0f, 0.0f, 1.0f
};
glm::vec2 translations[100];

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);

//render
void initGL();
GLuint initShaderProgram(const char* vertexShaderSource, const char* fragShaderSource);
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
	shaderProgram = initShaderProgram(bigTReadFileUtil.readFile("./simple_vertex_shader.vs"), bigTReadFileUtil.readFile("./simple_fragment_shader.fs"));
	initTriangle();
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
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	int index = 0;
	GLfloat offset = 0.1f;
	for (int y = -10; y < 10; y+=2)
	{
		for (int x = -10; x < 10; x+=2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}
	for (unsigned int i = 0; i < 100; i++)
	{
		std::stringstream ss;
		std::string index;
		ss << i;
		index = ss.str();
		GLint offsetsLocation = glGetUniformLocation(shaderProgram, ("offsets[" + index + "]").c_str());
		glUniform2fv(offsetsLocation, 1, &translations[i][0]);
	}
	
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//glUseProgram(shaderProgram);
	glBindVertexArray(triangleVAO);

	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);

	glfwSwapBuffers(window);
}