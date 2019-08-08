#include <iostream>
#include <string>

//gl
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//inner
#include "Shader.h"
#include "Camera.h"
#include "ReadFileUtil.h"
#include "Model.h"

//const
const int g_WindowWidth = 1280;
const int g_WindowHeight = 720;

int g_UseNormalMapping = 0;
// camera
Camera g_Camera(glm::vec3(0.0f, 10.0f, 30.0f));
bool g_Keys[1024];
float lastX = g_WindowWidth / 2.0f;
float lastY = g_WindowHeight / 2.0f;
bool firstMouse = true;
bool spacePress = false;
float fov = 45.0f;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Triangle
#define TRIANGLE_DEBUG 0;
GLuint g_QuadVao = 0;
GLuint g_QuadVbo = 0;
GLuint g_Texture = 0;
GLuint g_NormalTexture = 0;

void createQuad();
GLuint createTexture(const char* filename);
void drawQuad();

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(g_WindowWidth, g_WindowHeight, "bigT-camera", NULL, NULL);
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
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, keyCallback);

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD Init Failed\n");
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shader("./quad.vs", "./quad.fs");
	Shader lightShader("./light.vs", "./light.fs");
	Model sphere("../../../3dModel/Sphere.obj");
	createQuad();
	g_Texture = createTexture("../../../source_image/brickwall.jpg");
	g_NormalTexture = createTexture("../../../source_image/brickwall_normal.jpg");

	//Model nanosuitModel("D:/Code/practise/opengl-company/3dModel/Sphere.obj");

	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		{
			shader.use();

			glm::mat4 projection = glm::perspective(glm::radians(g_Camera.Zoom), (float)g_WindowWidth / (float)g_WindowHeight, 0.1f, 1000.0f);
			glm::mat4 view = g_Camera.GetViewMatrix();
			glm::mat4 model;
			model = glm::rotate(model, glm::radians((float)glfwGetTime() * -10.0f), glm::normalize(glm::vec3(0.0, 1.0, 0.0)));
			model = glm::translate(model, glm::vec3(-10.0f, 0.0f, 0.0f));
			shader.setMat4("model", model);
			shader.setMat4("view", view);
			shader.setMat4("projection", projection);
			shader.setVec3("lightPos", glm::vec3(0.0f, 10.0f, 0.0f));
			shader.setVec3("lightLookAt", glm::vec3(0.0f, 0.0f, 0.0f));
			shader.setVec3("viewPos", g_Camera.Position);
			shader.setInt("useNormalMapping", g_UseNormalMapping);
			shader.setInt("diffuseMap", 0);
			shader.setInt("normalMap", 1);

			drawQuad();
		}

		// 渲染灯
		{
			lightShader.use();
			glm::mat4 objectModel = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f));
			glm::mat4 view = g_Camera.GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(g_Camera.Zoom), (GLfloat)g_WindowWidth / (GLfloat)g_WindowHeight, 0.1f, 1000.0f);
			lightShader.setMat4("model", objectModel);
			lightShader.setMat4("view", view);
			lightShader.setMat4("projection", projection);

			sphere.Draw(lightShader);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//release glfw
	glfwTerminate();

	return 0;
}

GLuint createTexture(const char* filename)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// 加载并生成纹理
	stbi_set_flip_vertically_on_load(true);
	int w, h, ch;
	//unsigned char *data = stbi_load("./600x800.jpg", &w, &h, &ch, 0);
	unsigned char *data = stbi_load(filename, &w, &h, &ch, 0);
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

	return tex;
}

glm::vec3 calculateTangent(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 uv1, glm::vec2 uv2, glm::vec2 uv3)
{
	glm::vec3 tangent;

	glm::vec3 e1 = p2 - p1;
	glm::vec3 e2 = p3 - p2;

	glm::vec2 dtUv1 = uv2 - uv1;
	glm::vec2 dtUv2 = uv3 - uv2;

/*
	GLfloat f = 1.0f / (dtUv1.x * dtUv1.y - dtUv2.x * dtUv2.y);

	tangent.x = f * (dtUv2.y * e1.x - dtUv1.y * e2.x);
	tangent.y = f * (dtUv2.y * e1.y - dtUv1.y * e2.y);
	tangent.z = f * (dtUv2.y * e1.z - dtUv1.y * e2.z);
	tangent = glm::normalize(tangent);*/

	glm::mat2x2 matUv(dtUv1, dtUv2);
	glm::mat2x2 matUvInverse = glm::inverse(matUvInverse);

	glm::mat2x3 matE(e1, e2);
	glm::mat2x3 matTB = matE * matUvInverse;

	tangent = matTB[0];

	return tangent;
}

void createQuad()
{
	glGenVertexArrays(1, &g_QuadVao);
	glBindVertexArray(g_QuadVao);

	glGenBuffers(1, &g_QuadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, g_QuadVbo);
/*
	GLfloat vertices[] =
	{
		//顶点						法线					uv
		-10.0f, -10.0f, -10.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,
		-10.0f, 10.0f, -10.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,
		10.0f,  -10.0f, -10.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,
		10.0f, 10.0f, -10.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,
	};*/
	glm::vec3 pos1(-10.0f, 0.0f, 10.0f);
	glm::vec3 pos2(-10.0f, 0.0f, -10.0f);
	glm::vec3 pos3(10.0f, 0.0f, 10.0f);
	glm::vec3 pos4(10.0f, 0.0f, -10.0f);

	glm::vec2 uv1(0.0f, 0.0f);
	glm::vec2 uv2(0.0f, 1.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);

	glm::vec3 nm(0.0f, 1.0f, 0.0f);

	glm::vec3 tangent1 = calculateTangent(pos1, pos2, pos3, uv1, uv2, uv3);
	glm::vec3 tangent2 = calculateTangent(pos2, pos3, pos4, uv2, uv3, uv4);

	GLfloat vertices[] =
	{
		//顶点					法线					uv				切线
		pos1.x, pos1.y, pos1.z,	nm.x, nm.y, nm.z,	uv1.x, uv1.y,	tangent1.x, tangent1.y, tangent1.z,
		pos2.x, pos2.y, pos2.z,	nm.x, nm.y, nm.z,	uv2.x, uv2.y,	tangent1.x, tangent1.y, tangent1.z,
		pos3.x, pos3.y, pos3.z,	nm.x, nm.y, nm.z,	uv3.x, uv3.y,	tangent1.x, tangent1.y, tangent1.z,
								
		pos2.x, pos2.y, pos2.z,	nm.x, nm.y, nm.z,	uv2.x, uv2.y,	tangent2.x, tangent2.y, tangent2.z,
		pos3.x, pos3.y, pos3.z,	nm.x, nm.y, nm.z,	uv3.x, uv3.y,	tangent2.x, tangent2.y, tangent2.z,
		pos4.x, pos4.y, pos4.z,	nm.x, nm.y, nm.z,	uv4.x, uv4.y,	tangent2.x, tangent2.y, tangent2.z,
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 6));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 11, (void*)(sizeof(GLfloat) * 8));
	glEnableVertexAttribArray(3);
}

void drawQuad()
{
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, g_Texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, g_NormalTexture);
		glBindVertexArray(g_QuadVao);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

//callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			g_Keys[key] = true;
		else if (action == GLFW_RELEASE)
			g_Keys[key] = false;
	}
}
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		spacePress = true;
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_RELEASE)
	{
		if (spacePress)
		{
			g_UseNormalMapping = !g_UseNormalMapping;
			printf("normal mapping: %d\n", g_UseNormalMapping);
			spacePress = false;
		}
	}

	float speed = 5;

	if (g_Keys[GLFW_KEY_W] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessKeyboard(FORWARD, deltaTime * speed);
	if (g_Keys[GLFW_KEY_S] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessKeyboard(BACKWARD, deltaTime * speed);
	if (g_Keys[GLFW_KEY_A] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessKeyboard(LEFT, deltaTime * speed);
	if (g_Keys[GLFW_KEY_D] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessKeyboard(RIGHT, deltaTime * speed);

	if (g_Keys[GLFW_KEY_W])
		g_Camera.ProcessKeyboard(FORWARD, deltaTime);
	if (g_Keys[GLFW_KEY_S])
		g_Camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (g_Keys[GLFW_KEY_A])
		g_Camera.ProcessKeyboard(LEFT, deltaTime);
	if (g_Keys[GLFW_KEY_D])
		g_Camera.ProcessKeyboard(RIGHT, deltaTime);

	GLfloat velocity = 300.0f * deltaTime;
	if (g_Keys[GLFW_KEY_UP] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessMouseMovement(0, velocity*speed);
	if (g_Keys[GLFW_KEY_DOWN] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessMouseMovement(0, -velocity*speed);
	if (g_Keys[GLFW_KEY_LEFT] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessMouseMovement(-velocity*speed, 0);
	if (g_Keys[GLFW_KEY_RIGHT] && g_Keys[GLFW_KEY_LEFT_SHIFT])
		g_Camera.ProcessMouseMovement(velocity*speed, 0);
	if (g_Keys[GLFW_KEY_UP])
		g_Camera.ProcessMouseMovement(0, velocity);
	if (g_Keys[GLFW_KEY_DOWN])
		g_Camera.ProcessMouseMovement(0, -velocity);
	if (g_Keys[GLFW_KEY_LEFT])
		g_Camera.ProcessMouseMovement(-velocity, 0);
	if (g_Keys[GLFW_KEY_RIGHT])
		g_Camera.ProcessMouseMovement(velocity, 0);
}
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	printf("yoffset=%f\n", yoffset);
// 	if (fov >= 1.0f && fov <= 45.0f)
// 		fov -= yoffset;
// 	if (fov <= 1.0f)
// 		fov = 1.0f;
// 	if (fov >= 45.0f)
// 		fov = 45.0f;
	g_Camera.ProcessMouseScroll(yoffset);
}