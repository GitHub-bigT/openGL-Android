#include <iostream>
#include <fstream>
#include <string>

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

//const
const int m_WindowWidth = 960;
const int m_WindowHeight = 600;

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
char* readFile(char* filePath);

void initGL();
void initQuad();
void initTexture();
GLuint initShader(const char* shaderSource, GLenum shaderType);
GLuint initProgram(const char* vertexShaderSource, const char* fragShaderSource);
void draw_scene(GLFWwindow *window);
void draw_Imgui(GLFWwindow *window);

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
	glfwSwapInterval(1); // Enable vsync
	//wglSwapIntervalEXT(1);

	//registe callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD Init Failed\n");
		return -1;
	}

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330 core";
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	//initGL
	initGL();

	while (!glfwWindowShouldClose(window))
	{
		//event
		glfwPollEvents();
		process_input(window);

		char window_title[50];
		sprintf(window_title, "First ImGui: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		glfwSetWindowTitle(window, window_title);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//render
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		draw_scene(window);

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int i = 10;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderInt("int", &i, 0, 20, "%d");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f, "%.3f", 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		draw_Imgui(window);

		glfwSwapBuffers(window);
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

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

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
	m_Program = initProgram(readFile("./Simple.vs"), readFile("./Simple.fs"));
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


	glUseProgram(m_Program);

	glBindVertexArray(m_QuadVao);
	glBindTexture(GL_TEXTURE_2D, m_RawTex);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glfwSwapBuffers(window);
}

void draw_Imgui(GLFWwindow *window)
{
	// Rendering
	ImGui::Render();
	int display_w, display_h;
	//glfwMakeContextCurrent(window);
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	//glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
	//glClear(GL_COLOR_BUFFER_BIT);
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}