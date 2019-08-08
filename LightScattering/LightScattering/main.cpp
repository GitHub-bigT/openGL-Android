#include <iostream>
#include <string>

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include "../Util/Model.h"
#include "../Util/Camera.h"
#include "../Util/Shader.h"

#define M_PI 3.14159265358979
#define DEGREES_TO_RADIANS(degrees)((M_PI * degrees)/180)

void RenderTextureWindow();
void RenderDepthSmallWindow();
void RenderQuad();
void RenderCube();
void RenderGround();
void RenderFloor();
void RenderLight(GLfloat *pointVertices, int length);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseMove_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void draw_Imgui(GLFWwindow *window);
unsigned int loadTexture(char const * path);

const GLuint SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 720;
// Camera
//Camera  camera(glm::vec3(12.0f, 18.0f, 45.0f));
Camera  camera(glm::vec3(0.0f, 6.0f, 35.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90, 0);
GLfloat lastX = SCREEN_WIDTH / 2.0;
GLfloat lastY = SCREEN_HEIGHT / 2.0;
bool    keys[1024];

bool cameraRotEnabled = false;
// Light attributes
glm::vec3 lightPos(3.0f, 18.0f, 0.0f);
// Deltatime
GLfloat deltaTime = 0.0f;	
GLfloat lastFrame = 0.0f;  	

GLuint floorTexture;

int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LightScattering", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseMove_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD Init Failed\n");
		return -1;
	}
	printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
		glGetString(GL_SHADING_LANGUAGE_VERSION));

	{
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
	}


	Shader simpleDepthShader("shadow_mapping_depth.vert", "shadow_mapping_depth.frag");
	Shader quadShader("quad.vert", "quad.frag");
	Shader floorShader("texture.vert", "texture.frag");
	Shader depthDisplayShader("depthDisplay.vert", "depthDisplay.frag");
	Shader postprocessingShader("postprocessing.vert", "postprocessing.frag");
	postprocessingShader.use();
	postprocessingShader.setInt("screenTexture", 0);
	postprocessingShader.setInt("screenDepth", 1);
	postprocessingShader.setInt("depthMap", 2);
	postprocessingShader.setInt("preComputeTexture", 3);
	Shader preComputeShader("pre_compute.vert", "pre_compute.frag");

	string a = "../3dModel/Sphere.obj";
	Model cyborg(a);
	string floorPath = "../source_image/wood.png";
	unsigned int floorTexture = loadTexture(floorPath.c_str());

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	GLuint depthMap;
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	{
		glGenFramebuffers(1, &depthMapFBO);
		glGenTextures(1, &depthMap);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GLuint screenFBO;
	GLuint screenTexture;
	GLuint screenDepth;
	{
		glGenFramebuffers(1, &screenFBO);
		glGenTextures(1, &screenTexture);
		glBindTexture(GL_TEXTURE_2D, screenTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glGenTextures(1, &screenDepth);
		glBindTexture(GL_TEXTURE_2D, screenDepth);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, screenTexture, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, screenDepth, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// pre-compute
	GLuint preComputeTexture;
	GLuint preComputeFBO;
	{
		glGenFramebuffers(1, &preComputeFBO);
		glGenTextures(1, &preComputeTexture);
		glBindTexture(GL_TEXTURE_2D, preComputeTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 512, 512, 0, GL_RGBA, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glBindFramebuffer(GL_FRAMEBUFFER, preComputeFBO);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, preComputeTexture, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	float m_fMaxTracingDistance = 100.0f;
	float m_fDistanceScaler = 60000.0f / m_fMaxTracingDistance;
	// rayleight,mie散射系数
	glm::vec4 m_f4RayleightBeta(5.8e-6f, 13.5e-6f, 33.1e-6f, 0.0f);
	glm::vec4 m_f4MieBeta(2.0e-5f, 2.0e-5f, 2.0e-5f, 0.0f);
	// Scale scattering coefficients to match the scene scale
	const float fRayleighBetaMultiplier = m_fDistanceScaler;
	m_f4RayleightBeta *= fRayleighBetaMultiplier;
	const float fMieBetaMultiplier = 0.005f * m_fDistanceScaler;
	m_f4MieBeta *= fMieBetaMultiplier;
	glm::vec4 m_f4SummTotalBeta = m_f4RayleightBeta + m_f4MieBeta;

	// pre-compute
	{
		glBindFramebuffer(GL_FRAMEBUFFER, preComputeFBO);
		glViewport(0, 0, 512, 512);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		preComputeShader.use();
		preComputeShader.setVec4("rayleighBeta", m_f4RayleightBeta);
		preComputeShader.setVec4("mieBeta", m_f4MieBeta);
		preComputeShader.setVec4("summTotalBeta", m_f4SummTotalBeta);
		preComputeShader.setFloat("m_fMaxTracingDistance", m_fMaxTracingDistance);
		RenderQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	while (!glfwWindowShouldClose(window))
	{
		static float inConeAngle = 0.0f;
		static float outConeAngle = 25.0f;
		static float coneDirectionArray[3] = { 0.0f, 0.0f, 0.0f };
		static ImVec4 color = ImColor(255, 255, 255, 255); //ImColor(114, 144, 154, 200);

		// imgui
		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			do_movement();

			char window_title[50];
			sprintf_s(window_title, "LightScattering: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			glfwSetWindowTitle(window, window_title);

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Param");
			ImGui::InputFloat3("Cone Direction", coneDirectionArray);
			ImGui::SliderFloat("In Cone Angle", &inConeAngle, 1.0f, outConeAngle, "%.3f");            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::SliderFloat("Out Cone Angle", &outConeAngle, inConeAngle, 180.0f, "%.3f");
			ImGui::ColorEdit3("Light Color", (float*)&color);

			//static bool showDemo = true;
			//ImGui::ShowDemoWindow(&showDemo);

			ImGui::End();
		}

		glEnable(GL_DEPTH_TEST);

		// 深度图
		glm::mat4 lightSpaceMatrix;
		float objectModelRotateRadius = 1.5f;
		float objectModel_x = sin(glfwGetTime()) * objectModelRotateRadius;
		float objectModel_z = cos(glfwGetTime()) * objectModelRotateRadius;
		//objectModelRotateRadius = 5.0f;
		//lightPos.x = sin(glfwGetTime()) * objectModelRotateRadius;
		//lightPos.z = cos(glfwGetTime()) * objectModelRotateRadius;
		glm::mat4 objectModel = glm::translate(glm::mat4(1.0f) , glm::vec3(objectModel_x, 5, objectModel_z));
		{
			simpleDepthShader.use();

			glm::mat4 lightProjection, lightView, model;
			GLfloat near_plane = 1.0f, far_plane = 100.0f;
			lightProjection = glm::perspective(45.0f, (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // Note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene.
			lightView = glm::lookAt(lightPos, glm::vec3(coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]), glm::vec3(0.0, 1.0, 0.0));
			lightSpaceMatrix = lightProjection * lightView;
			simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			simpleDepthShader.setMat4("model", model);
			simpleDepthShader.setVec3("lightPosInWorldSpace", lightPos.x, lightPos.y, lightPos.z);
			simpleDepthShader.setVec3("coneViewPosInWorldSpace", coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]);
			simpleDepthShader.setFloat("InConeAngle", inConeAngle);

			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
			glClear(GL_DEPTH_BUFFER_BIT);

			//RenderFloor();
			simpleDepthShader.setMat4("model", objectModel);
			cyborg.Draw(simpleDepthShader);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, screenFBO);
		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		floorShader.use();

		// 渲染模型
		{
			floorShader.setMat4("model", objectModel);
			floorShader.setMat4("view", view);
			floorShader.setMat4("projection", projection);
			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
			cyborg.Draw(floorShader);
		}

		// 渲染地面
		{
			glm::mat4 model;// = glm::translate(glm::mat4(1.0f), glm::vec3());
			floorShader.setMat4("model", model);
			floorShader.setMat4("view", view);
			floorShader.setMat4("projection", projection);
			floorShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			floorShader.setVec3("lightPos", lightPos);
			floorShader.setVec3("lightColor", color.x, color.y, color.z);
			floorShader.setVec3("coneDirection", coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]);
			floorShader.setFloat("cosInConeAngle", glm::cos(glm::radians(inConeAngle)));
			floorShader.setFloat("cosOutConeAngle", glm::cos(glm::radians(outConeAngle)));

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, floorTexture);
			floorShader.setInt("shadowMap", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			//caotu.Draw(floorShader);
			RenderFloor();
		}

		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(1.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			// 后处理窗口
			{
				postprocessingShader.use();

				glm::mat4 view = camera.GetViewMatrix();
				float m_fNear = 0.1f;
				float m_fFar = 1000.0f;
				glm::vec4 m_f4ZBufferParams = glm::vec4(1.0f - m_fFar/m_fNear, m_fFar/m_fNear, (1.0f - m_fFar / m_fNear)/m_fFar, (m_fFar / m_fNear)/m_fFar);
				postprocessingShader.setVec4("m_f4ZBufferParams", m_f4ZBufferParams);

				glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, m_fNear, m_fFar);

				glm::mat4 viewInv = glm::inverse(view);
				glm::mat4 projInv = glm::inverse(projection);
				postprocessingShader.setMat4("viewInv", viewInv);
				postprocessingShader.setMat4("projInv", projInv);
				postprocessingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
				postprocessingShader.setMat4("cameraProj", projection);
				glm::mat4 screenSpaceMatrix;
				screenSpaceMatrix = projection * view;
				postprocessingShader.setMat4("screenSpaceMatrix", screenSpaceMatrix);

				postprocessingShader.setFloat("coneAngle", outConeAngle);
				postprocessingShader.setVec3("coneDirection", glm::vec3(coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]));

				postprocessingShader.setVec3("lightPos", lightPos);
				postprocessingShader.setVec3("lightColor", glm::vec3(color.x, color.y, color.z));
				postprocessingShader.setVec3("viewPos", camera.Position);

				glm::vec3 m_f3SpotLightAxis = glm::normalize(glm::vec3(coneDirectionArray[0] - lightPos.x, coneDirectionArray[1] - lightPos.y, coneDirectionArray[2] - lightPos.z));
				float m_fSpotCosAngle = glm::cos(glm::radians(outConeAngle));
				glm::vec4 m_f4SpotLightAxisAndCosAngle(m_f3SpotLightAxis, m_fSpotCosAngle);
				postprocessingShader.setVec4("f4SpotLightAxisAndCosAngle", m_f4SpotLightAxisAndCosAngle);

				postprocessingShader.setVec4("rayleighBeta", m_f4RayleightBeta);
				postprocessingShader.setVec4("mieBeta", m_f4MieBeta);
				postprocessingShader.setVec4("summTotalBeta", m_f4SummTotalBeta);
				postprocessingShader.setFloat("m_fMaxTracingDistance", m_fMaxTracingDistance);

				glm::mat4 model;
				glm::mat4 viewV;
				glm::mat4 projectionV;
				postprocessingShader.setMat4("model", model);
				postprocessingShader.setMat4("view", viewV);
				postprocessingShader.setMat4("projection", projectionV);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, screenTexture);
				glActiveTexture(GL_TEXTURE1);
				glBindTexture(GL_TEXTURE_2D, screenDepth);
				glActiveTexture(GL_TEXTURE2);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				glActiveTexture(GL_TEXTURE3);
				glBindTexture(GL_TEXTURE_2D, preComputeTexture);
				RenderQuad();
			}

			// 深度可视化窗口
			{
				depthDisplayShader.use();
				
				glm::mat4 model;
				glm::mat4 view;
				glm::mat4 projection;
				depthDisplayShader.setMat4("model", model);
				depthDisplayShader.setMat4("view", view);
				depthDisplayShader.setMat4("projection", projection);
				
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, depthMap);
				RenderDepthSmallWindow();
			}

			// 纹理可视化窗口
			{
				quadShader.use();

				glm::mat4 model;
				glm::mat4 view;
				glm::mat4 projection;
				quadShader.setMat4("model", model);
				quadShader.setMat4("view", view);
				quadShader.setMat4("projection", projection);

				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, preComputeTexture);
				RenderTextureWindow();
				//depthDisplayShader.use();
				//
				//glm::mat4 model;
				//glm::mat4 view;
				//glm::mat4 projection;
				//depthDisplayShader.setMat4("model", model);
				//depthDisplayShader.setMat4("view", view);
				//depthDisplayShader.setMat4("projection", projection);
				//
				//glActiveTexture(GL_TEXTURE0);
				//glBindTexture(GL_TEXTURE_2D, screenDepth);
				//RenderTextureWindow();
			}
		}

		draw_Imgui(window);

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	return 0;
}

void RenderFloor()
{
	// plane VAO
	static GLuint planeVAO = 0, planeVBO;
	if (planeVAO == 0)
	{
		float planeVertices[] = {
			// positions            // normals         // texcoords
			100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
			-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,

			100.0f, -0.5f,  100.0f,  0.0f, 1.0f, 0.0f,  10.0f,  0.0f,
			-100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,   0.0f, 10.0f,
			100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f,

			100.0f, -100.0f,  -100.0f,  0.0f, 0.0f, 1.0f,  10.0f,  0.0f,
			-100.0f, -100.0f,  -100.0f,  0.0f, 0.0f, 1.0f,   0.0f,  0.0f,
			-100.0f, 100.0f, -100.0f,  0.0f, 0.0f, 1.0f,   0.0f, 10.0f,

			-100.0f, 100.0f,  -100.0f,  0.0f, 0.0f, 1.0f,  10.0f,  0.0f,
			100.0f, 100.0f, -100.0f,  0.0f, 0.0f, 1.0f,   0.0f, 10.0f,
			100.0f, -100.0f, -100.0f,  0.0f, 0.0f, 1.0f,  10.0f, 10.0f
		};

		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}
	
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void RenderDepthSmallWindow()
{
	static GLuint smallWindowVAO = 0;
	static GLuint smallWindowVBO;
	if (smallWindowVAO == 0)
	{
		GLfloat quadVertices[] = {
			0.5f,  1.0f, 0.0f, 0.0f, 1.0f,
			0.5f,  0.5f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f,  0.5f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &smallWindowVAO);
		glGenBuffers(1, &smallWindowVBO);
		glBindVertexArray(smallWindowVAO);
		glBindBuffer(GL_ARRAY_BUFFER, smallWindowVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(smallWindowVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderTextureWindow()
{
	static GLuint textureWindowVAO = 0;
	static GLuint textureWindowVBO;
	if (textureWindowVAO == 0)
	{
		GLfloat quadVertices[] = {
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f,  0.5f, 0.0f, 0.0f, 0.0f,
			-0.5f,  1.0f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
		glGenVertexArrays(1, &textureWindowVAO);
		glGenBuffers(1, &textureWindowVBO);
		glBindVertexArray(textureWindowVAO);
		glBindBuffer(GL_ARRAY_BUFFER, textureWindowVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(textureWindowVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderQuad()
{
	static GLuint quadVAO = 0;
	static GLuint quadVBO;
	if (quadVAO == 0)
	{
		GLfloat quadVertices[] = {
			-1.0f,  1.0f, 0.1f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.1f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.1f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.1f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void RenderLight(GLfloat *pointVertices, int size)
{
	static GLuint pointVAO = 0;
	static GLuint pointVBO;
	int verCount = size / (sizeof(GLfloat) * 7);
	if (pointVAO == 0)
	{
		glGenVertexArrays(1, &pointVAO);
		glGenBuffers(1, &pointVBO);
		glBindVertexArray(pointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
		glBufferData(GL_ARRAY_BUFFER, size, pointVertices, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	}
	else
	{
		glBindVertexArray(pointVAO);
		glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
		glBufferData(GL_ARRAY_BUFFER, size, pointVertices, GL_DYNAMIC_DRAW);
		glBindVertexArray(0);
	}

	glBindVertexArray(pointVAO);
	glPointSize(10);
	glDrawArrays(GL_POINTS, 0, verCount);
	glBindVertexArray(0);
}

void RenderCube()
{
	static GLuint cubeVAO = 0;
	static GLuint cubeVBO = 0;
	// Initialize (if necessary)
	if (cubeVAO == 0)
	{
		GLfloat vertices[] = {
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};



		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// Fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// Link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// Render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void RenderGround()
{
	static GLuint groundVAO = 0;
	static GLuint groundVBO = 0;
	if (groundVAO == 0)
	{
		GLfloat vertices[] = {
			-0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
			0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.0f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.0f, -0.5f,  0.0f,  1.0f,  0.0f
		};



		glGenVertexArrays(1, &groundVAO);
		glGenBuffers(1, &groundVBO);
		glBindBuffer(GL_ARRAY_BUFFER, groundVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glBindVertexArray(groundVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	glBindVertexArray(groundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void do_movement()
{
	if (keys[GLFW_KEY_W] && keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(FORWARD, deltaTime * 5);
	if (keys[GLFW_KEY_S] && keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(BACKWARD, deltaTime * 5);
	if (keys[GLFW_KEY_A] && keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(LEFT, deltaTime * 5);
	if (keys[GLFW_KEY_D] && keys[GLFW_KEY_LEFT_SHIFT])
		camera.ProcessKeyboard(RIGHT, deltaTime * 5);

	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);

	GLfloat velocity = 100.0f * deltaTime;
	if (keys[GLFW_KEY_UP])
		camera.ProcessMouseMovement(0, velocity);
	if (keys[GLFW_KEY_DOWN])
		camera.ProcessMouseMovement(0, -velocity);
	if (keys[GLFW_KEY_LEFT])
		camera.ProcessMouseMovement(-velocity, 0);
	if (keys[GLFW_KEY_RIGHT])
		camera.ProcessMouseMovement(velocity, 0);
}

bool firstMouse = true;
void mouseMove_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  

	lastX = xpos;
	lastY = ypos;
	//if (cameraRotEnabled)
	//	camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouseButton_callback(GLFWwindow * window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		cameraRotEnabled = true;
	}
	else if (button == GLFW_RELEASE) {
		cameraRotEnabled = false;
	}
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	//printf("yoffset=%f\n", yoffset);
	camera.ProcessMouseScroll(yoffset);
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

unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}