#include <iostream>
#include <fstream>
#include <string>
//#include <QImage>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//const
const int m_WindowWidth = 900;
const int m_WindowHeight = 450;

GLuint m_RawProgram;
GLuint m_RawVao;
GLuint m_RawVbo;
GLuint m_RawTex;
GLfloat m_RawVertices[] =
{
	-1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.0f,  -1.0f, 1.0f, 1.0f, 0.0f,
	-1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.0f,  -1.0f, 1.0f, 1.0f, 0.0f
};

GLuint m_BeautyProgram;
GLuint m_BeautyVao;
GLuint m_BeautyVbo;
GLuint m_BeautyTex;
GLuint m_BeautyCurveTex;
GLfloat m_BeautyVertices[] =
{
	0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	1.0f,  -1.0f, 1.0f, 1.0f, 0.0f
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
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

	if (glIsBuffer(m_RawVbo))
	{
		glDeleteBuffers(1, &m_RawVbo);
		m_RawVbo = 0;
	}

	if (glIsVertexArray(m_RawVao))
	{
		glDeleteVertexArrays(1, &m_RawVao);
		m_RawVao = 0;
	}

	if (glIsTexture(m_RawTex))
	{
		glDeleteTextures(1, &m_RawTex);
		m_RawTex = 0;
	}

	if (glIsProgram(m_RawProgram))
	{
		glDeleteProgram(m_RawProgram);
		m_RawProgram = 0;
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
	m_RawProgram = initProgram(readFile("./Simple.vs"), readFile("./Simple.fs"));
	m_BeautyProgram = initProgram(readFile("./Simple.vs"), readFile("./beauty2.fs"));
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
	glGenVertexArrays(1, &m_RawVao);
	glBindVertexArray(m_RawVao);

	glGenBuffers(1, &m_RawVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_RawVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_RawVertices), m_RawVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &m_BeautyVao);
	glBindVertexArray(m_BeautyVao);

	glGenBuffers(1, &m_BeautyVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_BeautyVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_BeautyVertices), m_BeautyVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void initTexture()
{
	{
		glGenTextures(1, &m_RawTex);
		glBindTexture(GL_TEXTURE_2D, m_RawTex);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
// 		QImage img = QImage(QString(filename));
// 		QImage::Format fmt = img.format();
// 		img = img.convertToFormat(QImage::Format_RGBA8888);
		// 加载并生成纹理
		stbi_set_flip_vertically_on_load(false);
		int w, h, ch;
		unsigned char *data = stbi_load("./src.jpg", &w, &h, &ch, 0);
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
	
	{
		glGenTextures(1, &m_BeautyTex);
		glBindTexture(GL_TEXTURE_2D, m_BeautyTex);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 加载并生成纹理
		stbi_set_flip_vertically_on_load(false);
		int w, h, ch;
		unsigned char *data = stbi_load("./src.jpg", &w, &h, &ch, 0);
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

	{
		glGenTextures(1, &m_BeautyCurveTex);
		glBindTexture(GL_TEXTURE_2D, m_BeautyCurveTex);
		// 为当前绑定的纹理对象设置环绕、过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// 加载并生成纹理
		stbi_set_flip_vertically_on_load(false);
		int w, h, ch;
		unsigned char *data = stbi_load("./curve.jpg", &w, &h, &ch, 0);
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
}

void draw_scene(GLFWwindow *window)
{
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	//raw
	glUseProgram(m_RawProgram);
	glBindTexture(GL_TEXTURE_2D, m_RawTex);

	glBindVertexArray(m_RawVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//beauty
	glUseProgram(m_BeautyProgram);
	glUniform1i(glGetUniformLocation(m_BeautyProgram, "raw_tex"), 0);
	glUniform1i(glGetUniformLocation(m_BeautyProgram, "beauty_curve_tex"), 1);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_BeautyTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, m_BeautyCurveTex);

	glBindVertexArray(m_BeautyVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(window);
}