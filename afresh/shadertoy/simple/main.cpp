#include <iostream>
#include <fstream>
#include <string>
#include <windows.h>

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

#include "Shader.h"
#include "util.h"

//const
const int m_WindowWidth = 960;
const int m_WindowHeight = 600;

GLfloat m_Vertices[] =
{
	-1.0f, -1.0f, -1.0f, 1.0f, 1.0f,  -1.0f,
	-1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f
};
Shader *m_Shader = 0;
GLuint m_QuadVao;
GLuint m_QuadVbo;

// 菜单
GLuint m_RawTex;
bool m_UseTransparent = false;

// ShaderToy参数
glm::vec4 iDate(0,0,0,0);
glm::vec4 iMouse(0,0,0,0);
glm::vec2 iResolution(m_WindowWidth, m_WindowHeight);

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

void initGL();
void initQuad();
void updateTexture(std::string path);

void draw_scene(GLFWwindow *window);
void draw_Imgui(GLFWwindow *window);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(m_WindowWidth, m_WindowHeight, "ShaderToy", NULL, NULL);
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
	glfwSetCursorPosCallback(window, mouse_callback);

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
	std::string fontDir(getExeDir() + "\\simhei.ttf");
	io.Fonts->AddFontFromFileTTF(fontDir.c_str(), 15.0f, NULL, io.Fonts->GetGlyphRangesChineseFull());

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 330 core";
	ImGui_ImplOpenGL3_Init(glsl_version);

	bool show_demo_window = true;
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

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		std::string textureName;
		OPENFILENAME opfn;

		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu(u8"文件"))
			{
				//printf("click file\n");
				if (ImGui::MenuItem(u8"选择纹理", "")) 
				{

					//printf("click open\n");
					WCHAR strFilename[MAX_PATH];//存放文件名  
												//初始化  
					ZeroMemory(&opfn, sizeof(OPENFILENAME));
					opfn.lStructSize = sizeof(OPENFILENAME);//结构体大小  
															//设置过滤  
					opfn.lpstrFilter = L"所有文件\0*.*\0文本文件\0*.txt\0MP3文件\0*.mp3\0";
					//默认过滤器索引设为1  
					opfn.nFilterIndex = 1;
					//文件名的字段必须先把第一个字符设为 \0  
					opfn.lpstrFile = strFilename;
					opfn.lpstrFile[0] = '\0';
					opfn.nMaxFile = sizeof(strFilename);
					//设置标志位，检查目录或文件是否存在  
					opfn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
					//opfn.lpstrInitialDir = NULL;  
					// 显示对话框让用户选择文件  
					if (GetOpenFileName(&opfn))
					{
						textureName = Wchar_tToString(opfn.lpstrFile);
						printf("texture name = %s\n", textureName.c_str());

						updateTexture(textureName.c_str());
						//在文本框中显示文件路径  
						//HWND hEdt = GetDlgItem(hDlg, IDC_EDTFILENAME);
						//SendMessage(hEdt, WM_SETTEXT, NULL, (LPARAM)strFilename);
					}
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			// 基于当前系统的当前日期/时间
			time_t now = time(0);
			//std::cout << "1970 到目前经过秒数:" << now << std::endl;
			tm *ltm = localtime(&now);
			char data[256];
			sprintf(data, u8"%d年%d月%d日%d时%d分%d秒", 1900+ltm->tm_year, 1+ltm->tm_mon, ltm->tm_mday, ltm->tm_hour, ltm->tm_min, ltm->tm_sec);
			iDate = glm::vec4(1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday, ltm->tm_hour*60 + ltm->tm_min*60 + ltm->tm_sec);
			
			static float f = 0.0f;
			static int i = 10;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text(data);               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox(u8"透明", &m_UseTransparent);

			ImGui::SliderInt("int", &i, 0, 20, "%d");
			ImGui::SliderFloat("float", &f, 0.0f, 1.0f, "%.3f", 5.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color


			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		{
			ImGui::Begin("Texture");
			ImTextureID my_tex_id = io.Fonts->TexID;
			float my_tex_w = (float)io.Fonts->TexWidth;
			float my_tex_h = (float)io.Fonts->TexHeight;

			static int pressed_count = 0;
			for (int i = 0; i < 4; i++)
			{
				ImGui::PushID(i);
				int frame_padding = -1 + i;     // -1 = uses default padding
				if (ImGui::ImageButton(my_tex_id, ImVec2(128, 128), ImVec2(0, 0), ImVec2(32.0f / my_tex_w, 32 / my_tex_h), frame_padding, ImColor(255, 0, 0, 255)))
					pressed_count += 1;
				ImGui::PopID();
				ImGui::SameLine();
			}

			ImGui::NewLine();
			ImGui::Text("Pressed %d times.", pressed_count);
			ImGui::End();
		}

		// ShaderToy Code
		{
			ImGui::Begin("ShaderToy Code");
			static bool read_only = false;
			static char text[1024 * 32] =
				"#version 330 core\n"
				"uniform vec2 iResolution;\n"
				"uniform float iTime;\n"
				"uniform vec4 iMouse;\n"
				"uniform vec4 iDate;\n"
				"uniform sampler2D tex;\n"
				"\n"
				"out vec4 ofragColor;\n"
				"\n"
				"void mainImage( out vec4 fragColor, in vec2 fragCoord );\n"
				"\n"
				"void main()\n"
				"{\n"
				"	ofragColor = texture(tex, gl_FragCoord.xy / iResolution);\n"
				"	//mainImage(ofragColor, gl_FragCoord.xy);\n"
				"}\n";
			ImGui::Checkbox("Read-only", &read_only);
			ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput | (read_only ? ImGuiInputTextFlags_ReadOnly : 0);
			ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16), flags);
			//static char buf[2048] = "";
			//static char buf[32] = u8"NIHONGO"; // <- this is how you would write it with C++11, using real kanjis
			//ImGui::InputText("UTF-8 input", buf, IM_ARRAYSIZE(buf));
			if (ImGui::Button(u8"编译"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			{
				std::string fragmentCode(text);
				if (m_Shader)
				{
					delete m_Shader;
					m_Shader = 0;
				}
				m_Shader = new Shader(fragmentCode);
			}
			ImGui::End();
		}

		//render
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);

		draw_scene(window);

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

	if (m_Shader)
		delete m_Shader;

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
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	iMouse.x = xpos;
	iMouse.y = ypos;
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		iMouse.z = xpos;
		iMouse.w = ypos;
		//printf("x=%f, y=%f\n", xpos, ypos);
	}
}

void initGL()
{
	glViewport(0, 0, m_WindowWidth, m_WindowHeight);
	//simpleShader = new Shader("Simple.vs", "Simple.fs");
	initQuad();
	updateTexture("./1920x1200.jpg");
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

void initQuad()
{
	glGenVertexArrays(1, &m_QuadVao);
	glBindVertexArray(m_QuadVao);

	glGenBuffers(1, &m_QuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_QuadVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void updateTexture(std::string path)
{
	if (glIsTexture(m_RawTex))
	{
		glDeleteTextures(1, &m_RawTex);
		m_RawTex = 0;
	}

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
	unsigned char *data = stbi_load(path.c_str(), &w, &h, &ch, 0);
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
	float iTime = glfwGetTime();
	//printf("iTime=%f\n", iTime);

	if (m_Shader)
	{
		m_Shader->use();
		m_Shader->setVec2("iResolution", iResolution);
		m_Shader->setFloat("iTime", iTime);
		m_Shader->setVec4("iMouse", iMouse);
		m_Shader->setVec4("iDate", iDate);
	}

	if (m_UseTransparent)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}

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