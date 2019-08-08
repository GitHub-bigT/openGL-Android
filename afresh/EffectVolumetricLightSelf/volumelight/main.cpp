#include <iostream>
#include <string>

//imgui
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

//#include <GL/gl3w.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp> 
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
//#include <SOIL.h>
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
void DefineGBuffer(GLuint &gBuffer, GLuint &gPosition, GLuint &gNormal, GLuint &gDepth);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseMove_callback(GLFWwindow* window, double xpos, double ypos);
void mouseButton_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void do_movement();
void draw_Imgui(GLFWwindow *window);
float* generateLightVertex(glm::vec3 lightPos, glm::vec3 coneDir, float coneAngle, float coneLength);
unsigned int loadTexture(char const * path);

const GLuint SCREEN_WIDTH = 800, SCREEN_HEIGHT = 600;
// Camera
//Camera  camera(glm::vec3(12.0f, 18.0f, 45.0f));
Camera  camera(glm::vec3(0.0f, 20.0f, 35.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90, -30);
GLfloat lastX = SCREEN_WIDTH / 2.0;
GLfloat lastY = SCREEN_HEIGHT / 2.0;
bool    keys[1024];

bool cameraRotEnabled = false;
// Light attributes
//glm::vec3 lightPos(16.5f, 15.0f, 15.0f);
glm::vec3 lightPos(3.0f, 15.0f, 0.0f);
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

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "volumelight", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseMove_callback);
	glfwSetMouseButtonCallback(window, mouseButton_callback);
	glfwSetScrollCallback(window, scroll_callback);

/*
	if (gl3wInit()) {
		fprintf(stderr, "failed to initialize OpenGL\n");
		return -1;
	}
	if (!gl3wIsSupported(3, 3)) {
		fprintf(stderr, "OpenGL 3.3 not supported\n");
		return -1;
	}*/
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
	Shader fillgbufferShader("gbuffer.vert", "gbuffer.frag");
	Shader lightingShader("basic_lighting.vert", "basic_lighting.frag");
	//Shader occludeShader("occlude.vert", "occlude.frag");
	Shader quadShader("quad.vert", "quad.frag");
	Shader lightShader("light.vert", "light.frag");
	Shader floorShader("texture.vert", "texture.frag");
	Shader coneMaskShader("coneMask.vert", "coneMask.frag");
	Shader depthDisplayShader("depthDisplay.vert", "depthDisplay.frag");

	lightingShader.use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "gPosition"), 0 );
	glUniform1i(glGetUniformLocation(lightingShader.Program, "gNormal"), 1);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "gDepth"), 2);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "depthMap"), 3);

	string a = "D:/Resource/3Dmodel/apple.obj";
	Model cyborg(a);
	string b = "D:/Resource/3Dmodel/caotu.obj";
	//Model caotu(b);
	string floorPath = "D:/Code/practise/opengl-company/source_image/wood.png";
	unsigned int floorTexture = loadTexture(floorPath.c_str());

	GLuint gBuffer, gPosition, gNormal, gDepth;
	DefineGBuffer(gBuffer, gPosition, gNormal, gDepth);

	GLuint coneMaskFBO;
	glGenFramebuffers(1, &coneMaskFBO);
	GLuint coneMask;
	glGenTextures(1, &coneMask);
	glBindTexture(GL_TEXTURE_2D, coneMask);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	//GLfloat borderColor_2[] = { 1.0, 1.0, 1.0, 1.0 };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor_2);

	glBindFramebuffer(GL_FRAMEBUFFER, coneMaskFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, coneMask, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	GLuint depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	while (!glfwWindowShouldClose(window))
	{
		//lightPos.y += 0.1f;
		static float inConeAngle = 12.0f;
		static float outConeAngle = 35.0f;
		static float coneDirectionArray[3] = { 0.0f, 0.0f, 0.0f };
		static ImVec4 color = ImColor(114, 144, 154, 200);

		{
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			glfwPollEvents();
			do_movement();

			char window_title[50];
			sprintf_s(window_title, "First ImGui: %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
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

		// 从光照0,0,0的位置渲染模型。存深度
		glm::mat4 lightSpaceMatrix;
		float objectModelRotateRadius = 1.5f;
		float objectModel_x = sin(glfwGetTime()) * objectModelRotateRadius;
		float objectModel_z = cos(glfwGetTime()) * objectModelRotateRadius;
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

		/*{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			quadShader.use();

			glm::mat4 model;
			glm::mat4 view;
			glm::mat4 projection;
			quadShader.setMat4("model", model);
			quadShader.setMat4("view", view);
			quadShader.setMat4("projection", projection);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, depthMap);
			//glBindTexture(GL_TEXTURE_2D, gDepth);
			RenderQuad();
		}*/

		glm::mat4 view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);

		// 渲染摄像机下的数据到G缓冲
/*
		{
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			fillgbufferShader.use();

			fillgbufferShader.setMat4("model", objectModel);
			fillgbufferShader.setMat4("view", view);
			fillgbufferShader.setMat4("projection", projection);

			cyborg.Draw(fillgbufferShader);

			glm::mat4 model;
			fillgbufferShader.setMat4("model", model);
			RenderFloor();

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}*/

		// 渲染椎体遮罩
		{
			glBindFramebuffer(GL_FRAMEBUFFER, coneMaskFBO);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			coneMaskShader.use();

			coneMaskShader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
			coneMaskShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);
			coneMaskShader.setVec3("coneDirection", coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]);

			glm::mat4 cameraSpaceMatrix = projection * view;
			coneMaskShader.setMat4("cameraSpaceMatrix", cameraSpaceMatrix);
			coneMaskShader.setVec3("light_position", lightPos.x, lightPos.y, lightPos.z);
			coneMaskShader.setVec3("cone_view_position", coneDirectionArray[0], coneDirectionArray[1], coneDirectionArray[2]);

			glm::mat4 invView, invProj;
			invView = glm::inverse(view);
			invProj = glm::inverse(projection);
			coneMaskShader.setFloat("coneAngle", inConeAngle);
			coneMaskShader.setMat4("invProjection", invProj);
			coneMaskShader.setMat4("invView", invView);
			coneMaskShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

			coneMaskShader.setInt("depthMap", 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, depthMap);

			RenderQuad();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		// 渲染光照模型
/*
		{
			lightShader.use();
			
			glm::mat4 model;
			lightShader.setMat4("model", model);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);
			lightShader.setVec3("lightColor", color.x, color.y, color.z);

			float lightPoint[] = { lightPos[0], lightPos[1], lightPos[2], 1,1,0, 1, };
			RenderLight(lightPoint, sizeof(lightPoint));
		}*/

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
			glBindTexture(GL_TEXTURE_2D, coneMask);
			RenderTextureWindow();
		}

		// 从摄像机0,0,10的位置渲染模型。存位置，法线，深度
		/*
		glm::mat4 view;
		view = camera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(camera.Zoom, (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		{
			glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			fillgbufferShader.use();

			GLint modelLoc = glGetUniformLocation(fillgbufferShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(fillgbufferShader.Program, "view");
			GLint projLoc = glGetUniformLocation(fillgbufferShader.Program, "projection");

			glm::mat4 model;// = glm::translate(glm::mat4(1.0f), glm::vec3(0, -0.5, 0))* glm::scale(glm::mat4(1.0f), glm::vec3(10));
			model = glm::scale(model, glm::vec3(1.0f));

			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			//	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			//	RenderGround();
			cyborg.Draw(fillgbufferShader);
		}

		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			lightingShader.use();

			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, gPosition);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, gNormal);
			glActiveTexture(GL_TEXTURE2);
			glBindTexture(GL_TEXTURE_2D, gDepth);
			glActiveTexture(GL_TEXTURE3);
			glBindTexture(GL_TEXTURE_2D, depthMap);

			glm::mat4 invView = glm::inverse(view);
			glm::mat4 invProj = glm::inverse(projection);
			//GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");
			GLint lightColorLoc = glGetUniformLocation(lightingShader.Program, "lightColor");
			GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "lightPos");
			GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
			GLint invViewLoc = glGetUniformLocation(lightingShader.Program, "invView");
			GLint invProjLoc = glGetUniformLocation(lightingShader.Program, "invProjection");

			GLint coneAngleLoc = glGetUniformLocation(lightingShader.Program, "coneAngle");
			glUniform1f(coneAngleLoc, coneAngle);
			GLint coneDirectionLoc = glGetUniformLocation(lightingShader.Program, "coneDirection");
			glUniform3f(coneDirectionLoc, coneDirection.x, coneDirection.y, coneDirection.z);

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "lightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(lightSpaceMatrix));
			//glUniform3f(objectColorLoc, 1.0f, 1.0f, 1.0f);
			glUniform3f(lightColorLoc, 0.0f, 1.0f, 0.0f);
			glUniform3f(lightPosLoc, lightPos.x, lightPos.y, lightPos.z);
			glUniform3f(viewPosLoc, camera.Position.x, camera.Position.y, camera.Position.z);
			glUniformMatrix4fv(invViewLoc, 1, GL_FALSE, glm::value_ptr(invView));
			glUniformMatrix4fv(invProjLoc, 1, GL_FALSE, glm::value_ptr(invProj));

			RenderQuad();
		}*/

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
			100.0f, -0.5f, -100.0f,  0.0f, 1.0f, 0.0f,  10.0f, 10.0f
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
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
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

float* generateLightVertex(glm::vec3 lightPos, glm::vec3 coneDir, float coneAngle, float coneLength)
{
	coneDir = glm::normalize(coneDir);

	glm::vec3 endPos = coneDir * coneLength + lightPos;

	return NULL;
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

void DefineGBuffer(GLuint &gBuffer, GLuint &gPosition, GLuint &gNormal, GLuint &gDepth){
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	// - Position color buffer
	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
	// - Normal color buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
	// - Deph Buffer
	glGenTextures(1, &gDepth);
	glBindTexture(GL_TEXTURE_2D, gDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gDepth, 0);

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	//// - Color + Specular color buffer
	//glGenTextures(1, &gAlbedoSpec);
	//glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
	// - Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering

	//// - Create and attach depth buffer (renderbuffer)
	//GLuint rboDepth;
	//glGenRenderbuffers(1, &rboDepth);
	//glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SCREEN_WIDTH, SCREEN_HEIGHT);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
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