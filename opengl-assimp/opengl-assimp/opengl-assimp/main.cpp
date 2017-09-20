// Std. Includes
#include <string>

// GLEW
#include <../GL/glew.h>

// GLFW
#include <../GLFW/glfw3.h>

// GL includes
#include "BTShader.h"
#include "BTCamera.h"
#include "Model.h"

// GLM Mathemtics
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>

// Other Libs
#include <../SOIL/include/SOIL.h>

// Properties
GLuint screenWidth = 800, screenHeight = 600;
GLfloat xoffset = 90.0f;
GLfloat yoffset;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 60.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
bool isClick = false;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat loadTime = 0.0f;
GLfloat startTime = 0.0f;
GLfloat endTime = 0.0f;
GLint flag = 1;

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	printf("===== start project =====\n");
	startTime = glfwGetTime();
	printf("��Ŀ���еĿ�ʼʱ�䣺%f\n",startTime);
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	// Setup and compile our shaders
	Shader ourShader("v_shader.vert", "f_shader.frag");

	// Load models
	//Model ourModel("../../../3dModel/ball/ball1.obj");
	Model ourModel("../../../3dModel/ball2.obj");

	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//���޳�
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		ourShader.Use();
		//printf("%f\n",camera.Position.z);
		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 300.0f);
		glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;

		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(ourShader);
		if (flag == 1)
		{
			endTime = glfwGetTime();
			printf("��Ŀ����ʱ�䣺%f\n", endTime);
			loadTime = endTime - startTime;
			printf("����model���ĵ�ʱ�䣺%f\n", loadTime);
		}
		flag++;

		// Swap the buffers
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

#pragma region "User input"

GLfloat reduce = 0.1f;
// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W]){
		camera.ProcessKeyBoard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S]){
		camera.ProcessKeyBoard(BACKWARD, deltaTime);
	}
	camera.updateCameraVectors();
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (isClick)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		xoffset += (xpos - lastX) * 0.1f;
		yoffset += (ypos - lastY) * 0.1f;

		lastX = xpos;
		lastY = ypos;
		if (yoffset >= 89.0f)
		{
			yoffset = 89.0f;
		}
		if (yoffset <= -89.0f)
		{
			yoffset = -89.0f;
		}
		camera.ProcessMouseMovement(xoffset, yoffset);
	}

}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (action == GLFW_PRESS)
	{
		isClick = true;
		firstMouse = true;
		switch (button)
		{
			case GLFW_MOUSE_BUTTON_RIGHT:
			printf("Mosue right button clicked!\n");
			break;
		default:
		return;
		}
	}
	else if (action == GLFW_RELEASE){
		isClick = false;
	}
		
	return;
}

#pragma endregion