#include <iostream>
#include <fstream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "Camera.h"

//const
const int m_WindowWidth = 960;
const int m_WindowHeight = 600;

Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
float lastX = m_WindowWidth / 2.0f;
float lastY = m_WindowHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLuint m_Program;
GLuint m_QuadVao;
GLuint m_QuadVbo;
GLuint m_RawTex;
GLfloat m_Vertices[] =
{
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
char* readFile(char* filePath);

void initGL();
void initQuad();
void initTexture();
GLuint initShader(const char* shaderSource, GLenum shaderType);
GLuint initProgram(const char* vertexShaderSource, const char* fragShaderSource);
void draw_scene(GLFWwindow *window);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "bigT-triangle", NULL, NULL);
	if (window == NULL)
	{
		printf("GLFW Window == NULL\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//registe callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

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
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//event
		glfwPollEvents();
		process_input(window);

		//render
		draw_scene(window);
	}

	if (glIsBuffer(m_QuadVbo))
	{
		glDeleteBuffers(1, &m_QuadVbo);
		m_QuadVbo = 0;
	}

	if (glIsVertexArray(m_QuadVao))
	{
		glDeleteVertexArrays(1, &m_QuadVao);
		m_QuadVao = 0;
	}

	if (glIsTexture(m_RawTex))
	{
		glDeleteTextures(1, &m_RawTex);
		m_RawTex = 0;
	}

	if (glIsProgram(m_Program))
	{
		glDeleteProgram(m_Program);
		m_Program = 0;
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
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

char* readFile(char* filePath)
{
	char* txt = nullptr;
	std::string fileContent;
	std::ifstream fileContentStream(filePath, std::ios::in);
	if (fileContentStream.is_open()) {
		std::string line = "";
		while (std::getline(fileContentStream, line)) {
			fileContent += "\n" + line;
		}
		fileContentStream.close();

		char* t = (char*)fileContent.c_str();
		txt = (char*)malloc(fileContent.length() * sizeof(char));
		strcpy(txt, t);
	}

	return txt;
}

void initGL()
{
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	m_Program = initProgram(readFile("./Simple.vs"), readFile("./Simple.fs"));
	//SRT
	glUseProgram(m_Program);
	glm::mat4 model;
	//model = glm::scale(model, glm::vec3((float)m_WindowWidth*1.5f, 1.0f, 1.0f));
	//model = glm::scale(model, glm::vec3(1.0f, (float)m_WindowHeight*1.5f, 1.0f));
	//model = glm::scale(model, glm::vec3(1.0f, 1.0f, -200.0f));
	//model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));//z +20
	//model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));//y +20
	//model = glm::rotate(model, glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//model = glm::rotate(model, glm::radians(-20.0f), glm::vec3(1.0f, 0.0f, 0.0f));//x +20
	//model = glm::rotate(model, glm::radians(20.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));//z +45, y +45
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));//z +45, x +45
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));//y +45, x +45
	//model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
	//model = glm::translate(model, glm::vec3((float)m_WindowWidth / (float)m_WindowHeight, 0.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(m_Program, "model"), 1, GL_FALSE, &model[0][0]);
	//
	glm::mat4 view;
	//view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(m_Program, "view"), 1, GL_FALSE, &view[0][0]);

	//glm::mat4 ortho;
	//glm::mat4 ortho = glm::orthoRH((float)-m_WindowWidth, (float)m_WindowWidth, (float)-m_WindowHeight, (float)m_WindowHeight, 0.1f, 100.0f);
	//glm::mat4 ortho = glm::orthoRH(-(float)m_WindowWidth / (float)m_WindowHeight, (float)m_WindowWidth / (float)m_WindowHeight, -1.0f, 1.0f, -1.0f, 1.0f);
	//glm::mat4 projection = glm::frustumRH((float)-m_WindowWidth, (float)m_WindowWidth, (float)-m_WindowHeight, (float)m_WindowHeight, 100.0f, 200.0f);
	//glm::mat4 projection = glm::perspective(45.0f, (float)m_WindowWidth / (float)m_WindowHeight, 10.0f, 100.0f);

	//glUniformMatrix4fv(glGetUniformLocation(m_Program, "projection"), 1, GL_FALSE, &projection[0][0]);

	initQuad();
	initTexture();
}

GLuint initShader(const char* shaderSource, GLenum shaderType)
{
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderSource, NULL);
	glCompileShader(shader);
	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		printf("COMPILE SHADER ERRPR,%s\n", std::string(infoLog).c_str());
	}
	else
		printf("COMPILE SHADER SUCCESS\n");
	return shader;
}

GLuint initProgram(const char* vertexShaderSource, const char* fragShaderSource)
{
	GLuint vertexShader = initShader(vertexShaderSource, GL_VERTEX_SHADER);
	GLuint fragShader = initShader(fragShaderSource, GL_FRAGMENT_SHADER);
	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragShader);
	glLinkProgram(program);
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
	glUseProgram(program);
	//delete shader
	glDeleteShader(vertexShader);
	glDeleteShader(fragShader);

	return program;
}

void initQuad()
{
	glGenVertexArrays(1, &m_QuadVao);
	glBindVertexArray(m_QuadVao);

	glGenBuffers(1, &m_QuadVbo);
	bool b1 = glIsBuffer(m_QuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	bool b2 = glIsBuffer(m_QuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	bool b3 = glIsBuffer(m_QuadVbo);
	glBindVertexArray(0);

/*
	GLuint vbo2[2];
	glGenBuffers(100, vbo2);
	bool b4 = glIsBuffer(m_QuadVbo);
	int a = 0;*/
}

void initTexture()
{
	glGenTextures(1, &m_RawTex);
	glBindTexture(GL_TEXTURE_2D, m_RawTex);
	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 加载并生成纹理
	stbi_set_flip_vertically_on_load(true);
	int w, h, ch;
	//unsigned char *data = stbi_load("./600x800.jpg", &w, &h, &ch, 0);
	unsigned char *data = stbi_load("D:/Resource/透明/menu_choices.png", &w, &h, &ch, 0);
	if (data)
	{
		GLint format = 0;
		if (ch == 4)
			format = GL_RGBA;
		if (ch == 3)
			format = GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	glBindTexture(GL_TEXTURE_2D, 0);
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(1.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindVertexArray(m_QuadVao);
	glBindTexture(GL_TEXTURE_2D, m_RawTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
}