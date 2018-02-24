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
const int windowWidth = 1280;
const int windowHeight = 720;

// camera
Camera camera(glm::vec3(0.0f, 10.0f, 50.0f));
float lastX = windowWidth / 2.0f;
float lastY = windowHeight / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//Triangle
#define TRIANGLE_DEBUG 0;

GLfloat skyboxVertices[] =
{
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	1.0f,  1.0f, -1.0f,
	1.0f,  1.0f,  1.0f,
	1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	1.0f, -1.0f,  1.0f
};

//callback fun
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

GLuint loadCubemap(std::vector<std::string> faces);

int main()
{
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "bigT-camera", NULL, NULL);
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

	//init glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("GLAD Init Failed\n");
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader ourShader("simple_vertex_shader.vs", "simple_fragment_shader.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	Shader nanosuitShader("1.model_loading.vs", "1.model_loading.fs");

	// load models
	// -----------
	Model nanosuitModel("nanosuit/nanosuit.obj");
	//skybox
	GLuint skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void*)0);
	std::vector<std::string> faces
	{
		"images/right.jpg",
		"images/left.jpg",
		"images/top.jpg",
		"images/bottom.jpg",
		"images/back.jpg",
		"images/front.jpg"
/*
		"mp_blood/blood_rt.tga",
		"mp_blood/blood_lf.tga",
		"mp_blood/blood_up.tga",
		"mp_blood/blood_dn.tga",
		"mp_blood/blood_bk.tga",
		"mp_blood/blood_ft.tga",*/
/*
		"mp_hanging/hangingstone_rt.tga",
		"mp_hanging/hangingstone_lf.tga",
		"mp_hanging/hangingstone_up.tga",
		"mp_hanging/hangingstone_dn.tga",
		"mp_hanging/hangingstone_bk.tga",
		"mp_hanging/hangingstone_ft.tga"*/
	};
	GLuint skyboxTexture = loadCubemap(faces);

	//skyboxShader.use();
	//skyboxShader.setInt("skybox", 0);

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

		glDepthMask(GL_TRUE);
		// render the loaded model
		nanosuitShader.use();
		glm::mat4 nanoModel;
		//nanoModel = glm::translate(nanoModel, glm::vec3(0.0f, -5.5f, -5.5f));
		nanoModel = glm::rotate(nanoModel, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 nanosuitView = camera.GetViewMatrix();
		glm::mat4 nanosuitProjection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		nanosuitShader.setMat4("model", nanoModel);
		nanosuitShader.setMat4("view", nanosuitView);
		nanosuitShader.setMat4("projection", nanosuitProjection);
		nanosuitShader.setVec3("cameraPos", camera.Position);
		nanosuitModel.Draw(nanosuitShader);

		glDepthFunc(GL_LEQUAL);
		//glDepthMask(GL_FALSE);
		skyboxShader.use();
		glm::mat4 skyboxModel;
		//skyboxModel = glm::rotate(skyboxModel, (float)glfwGetTime(), glm::vec3(0.0f, 1.0f, 0.0f));
		//skyboxModel = glm::scale(skyboxModel, glm::vec3(10.0f, 10.0f, 10.0f));
		//skyboxModel = glm::translate(skyboxModel, glm::vec3(0.0f, 0.5f, 0.0f));
		glm::mat4 skyboxView = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glm::mat4 skyboxProjection = glm::perspective(glm::radians(camera.Zoom), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
		skyboxShader.setMat4("model", skyboxModel);
		skyboxShader.setMat4("view", skyboxView);
		skyboxShader.setMat4("projection", skyboxProjection);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP , skyboxTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//release glfw
	glfwTerminate();

	return 0;
}

GLuint loadCubemap(std::vector<std::string> faces)
{
	GLuint textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, channels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &channels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	return textureId;
}

//callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
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

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}