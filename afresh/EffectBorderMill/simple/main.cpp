#include <iostream>
#include <fstream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//const millumin2
float m_BorderWidth = 0.0f;
float m_Start = 0.0f;
float m_End = 100.0f;
float m_Phase = 0.0f;
const int m_WindowWidth = 960;
const int m_WindowHeight = 600;

GLuint m_QuadProgram;
GLuint m_QuadVao;
GLuint m_QuadVbo;
GLuint m_LineProgram;
GLuint m_LineVao;
GLuint m_LineVbo;
GLuint m_LineEbo;
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

GLfloat m_Lines[] =
{
	//上
	-1.0f, 1.0f,//左下0,1
	-1.0f, 1.0f,
	1.0f, 1.0f,
	1.0f, 1.0f,//右下6,7
	//下
	-1.0f, -1.0f,
	-1.0f, -1.0f,//左上10,11
	1.0f, -1.0f,//右上12,13
	1.0f, -1.0f,
	//左
	-1.0f, -1.0f,
	-1.0f, 1.0f,
	-1.0f, 1.0f,//右上20,21
	-1.0f, -1.0f,//右下22,23
	//右
	1.0f, -1.0f,//左下24,25
	1.0f, 1.0f,//左上26,27
	1.0f, 1.0f,
	1.0f, -1.0f
	//补
	- 1.0f, -1.0f,//左下32,33
	-1.0f, -1.0f,
	-1.0f, -1.0f,//右上
	-1.0f, -1.0f,//右下
};

GLuint m_LinesEbos[] = 
{
	//上
	0, 1, 3,
	1, 2, 3,
	//下
	4, 5, 7,
	5, 6, 7,
	//左
	8, 9, 11,
	9, 10, 11,
	//右
	12, 13, 15,
	13, 14, 15
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
char* readFile(char* filePath);

void initGL();
void initQuad();
void initLine();
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

	if (glIsProgram(m_QuadProgram))
	{
		glDeleteProgram(m_QuadProgram);
		m_QuadProgram = 0;
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

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		m_BorderWidth += 1.0f;
		printf("border width = %f\n", m_BorderWidth);
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		m_BorderWidth -= 1.0f;
		if (m_BorderWidth < 0.0f)
		{
			m_BorderWidth = 0.0f;
		}
		printf("border width = %f\n", m_BorderWidth);
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		m_Start += 1.0f;
		if (m_Start > m_End)
		{
			m_Start = m_End;
		}
		printf("m_Start = %f\n", m_Start);
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		m_Start -= 1.0f;
		if (m_Start < 0.0f)
		{
			m_Start = 0.0f;
		}
		printf("m_Start = %f\n", m_Start);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		m_End += 1.0f;
		if (m_End > 100.0f)
		{
			m_End = 100.0f;
		}
		printf("m_End = %f\n", m_End);
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		m_End -= 1.0f;
		if (m_End < m_Start)
		{
			m_End = m_Start;
		}
		printf("m_End = %f\n", m_End);
	}

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		m_Phase += 1.0f;
		m_Start += 1.0f;
		m_End += 1.0f;
		if (m_End > 100.0f)
		{
			m_End = 100.0f;
		}
		if (m_Start > m_End)
		{
			m_Start = m_End;
		}
		printf("m_Phase = %f\n", m_Phase);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		m_Phase -= 1.0f;
		m_Start -= 1.0f;
		m_End -= 1.0f;
		if (m_Start < 0.0f)
		{
			m_Start = 0.0f;
		}
		if (m_End < m_Start)
		{
			m_End = m_Start;
		}
		printf("m_Phase = %f\n", m_Phase);
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
	m_QuadProgram = initProgram(readFile("./Simple.vs"), readFile("./Simple.fs"));
	m_LineProgram = initProgram(readFile("./Line.vs"), readFile("./Line.fs"));
	initQuad();
	initLine();
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

void initLine()
{
	glGenVertexArrays(1, &m_LineVao);
	glBindVertexArray(m_LineVao);

	glGenBuffers(1, &m_LineVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Lines), m_Lines, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_LineEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_LineEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_LinesEbos), m_LinesEbos, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void initQuad()
{
	glGenVertexArrays(1, &m_QuadVao);
	glBindVertexArray(m_QuadVao);

	glGenBuffers(1, &m_QuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
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
	unsigned char *data = stbi_load("./1920x1200.jpg", &w, &h, &ch, 0);
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
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(m_QuadProgram);
	glBindVertexArray(m_QuadVao);
	glBindTexture(GL_TEXTURE_2D, m_RawTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	if (m_BorderWidth > 0.0f)
	{
		m_Lines[1] = m_Lines[7] = 1.0f - m_BorderWidth / (float)m_WindowHeight;
		m_Lines[11] = m_Lines[13] = -1.0f + m_BorderWidth / (float)m_WindowHeight;
		m_Lines[20] = m_Lines[22] = -1.0f + m_BorderWidth / (float)m_WindowWidth;//左
		m_Lines[24] = m_Lines[26] = 1.0f - m_BorderWidth / (float)m_WindowWidth;
	}

	if (m_Start > 0.0f)
	{
		float total = (m_WindowWidth * 2 + m_WindowHeight * 2) * (m_Start / 100.0f);
		//left
		if (total > m_WindowHeight)
			m_Lines[17] = m_Lines[23] = 1.0f;
		else
			m_Lines[17] = m_Lines[23] = -1.0f + total / (float)m_WindowHeight * 2;
		total -= m_WindowHeight;
		//top
		if (total > m_WindowWidth)
			m_Lines[0] = m_Lines[2] = 1.0f;
		else if(total > 0)
			m_Lines[0] = m_Lines[2] = -1.0f + total / (float)m_WindowWidth * 2;
		else
			m_Lines[0] = m_Lines[2] = -1.0f;
		total -= m_WindowWidth;
		//right
		if (total > m_WindowHeight)
			m_Lines[27] = m_Lines[29] = -1.0f;
		else if (total > 0)
			m_Lines[27] = m_Lines[29] = 1.0f - total / (float)m_WindowHeight * 2;
		else
			m_Lines[27] = m_Lines[29] = 1.0f;
		total -= m_WindowHeight;
		//bottom
		if (total >= m_WindowWidth)
			m_Lines[12] = m_Lines[14] = -1.0f;
		else if (total > 0)
			m_Lines[12] = m_Lines[14] = 1.0f - total / (float)m_WindowWidth * 2;
		else
			m_Lines[12] = m_Lines[14] = 1.0f;
		total -= m_WindowWidth;
	}
	else
	{
		m_Lines[17] = m_Lines[23] = -1.0f;
	}

	if (m_End < 100.0f)
	{
		float total = (m_WindowWidth * 2 + m_WindowHeight * 2) * (1.0f - (m_End / 100.0f));
		//bottom
		if (total >= m_WindowWidth)
			m_Lines[8] = m_Lines[10] = 1.0f;
		else if (total > 0)
			m_Lines[8] = m_Lines[10] = -1.0f + total / (float)m_WindowWidth * 2;
		else
			m_Lines[8] = m_Lines[10] = -1.0f;
		total -= m_WindowWidth;
		//right
		if (total > m_WindowHeight)
			m_Lines[25] = m_Lines[31] = 1.0f;
		else if (total > 0)
			m_Lines[25] = m_Lines[31] = -1.0f + total / (float)m_WindowHeight * 2;
		else
			m_Lines[25] = m_Lines[31] = -1.0f;
		total -= m_WindowHeight;
		//top
		if (total > m_WindowWidth)
			m_Lines[4] = m_Lines[6] = -1.0f;
		else if (total > 0)
			m_Lines[4] = m_Lines[6] = 1.0f - total / (float)m_WindowWidth * 2;
		else
			m_Lines[4] = m_Lines[6] = 1.0f;
		total -= m_WindowWidth;
		//left
		if (total > m_WindowHeight)
			m_Lines[19] = m_Lines[21] = -1.0f;
		else if (total > 0)
			m_Lines[19] = m_Lines[21] = 1.0f - total / (float)m_WindowHeight * 2;
		else
			m_Lines[19] = m_Lines[21] = 1.0f;
		total -= m_WindowHeight;
	}
	else
	{
		m_Lines[8] = m_Lines[10] = -1.0f;

	}

	glBindBuffer(GL_ARRAY_BUFFER, m_LineVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Lines), m_Lines, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(m_LineProgram);
	glBindVertexArray(m_LineVao);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);

	glfwSwapBuffers(window);
}