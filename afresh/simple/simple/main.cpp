#include <iostream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
//inner
#include "ShaderUtil.h"
#include "ReadFileUtil.h"

//const
const int windowWidth = 800;
const int windowHeight = 600;
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
	-0.5f, 1.0f, 0.0f,
	-1.0f, 0.0f, 0.0f,
	0.0f,  0.0f, 0.0f
};

GLuint triangleVBO2;
GLfloat triangleVertices2[] =
{
	0.5f, 0.0f, 1.0f,
	0.0f, -1.0f, 1.0f,
	1.0f,  -1.0f, 1.0f
};

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
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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
	glGenBuffers(1, &triangleVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &triangleVBO2);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices2), triangleVertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenVertexArrays(1, &triangleVAO);
	glBindVertexArray(triangleVAO);
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, triangleVBO2);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgram);
/*
	glm::mat4 model;
	model = glm::scale(model, glm::vec3(800.0f, 600.0f, 1.0f));
	GLuint mPosi = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(mPosi, 1, GL_FALSE, &model[0][0]);*/

	//
	//glm::mat4 projectopn1 = glm::perspective(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 1000.0f);
	//glm::mat4 ortho = glm::orthoRH(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	//glm::mat4 projection = glm::frustumRH(-1.0f, 1.0f, -1.0f, 1.0f, 9.9f, 10.0f);
	//glm::mat4 projection1 = glm::perspective(glm::radians(90.0f), 800.0f / 600.0f, 0.0f, 1000.0f);
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(2.0f, 2.0f, 0.0f));

	//GLuint pPosi = glGetUniformLocation(shaderProgram, "projection");
	//glUniformMatrix4fv(pPosi, 1, GL_FALSE, &projection[0][0]);
	glBindVertexArray(triangleVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

// 	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
// 	glEnableVertexAttribArray(0);
// 	glDrawArrays(GL_TRIANGLES, 0, 3);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 
// 	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO2);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
// 	glEnableVertexAttribArray(0);
// 	glDrawArrays(GL_TRIANGLES, 0, 3);
// 	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glfwSwapBuffers(window);
}