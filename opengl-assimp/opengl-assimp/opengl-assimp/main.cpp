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

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

GLfloat loadTime = 0.0f;
GLfloat startTime = 0.0f;
GLfloat endTime = 0.0f;
GLint flag = 1;

//左右旋转
GLfloat angleH = 0.0f;
//上下旋转
GLfloat angleV = 0.0f;
// The MAIN function, from here we start our application and run our Game loop
int main()
{
	printf("===== start project =====\n");
	startTime = glfwGetTime();
	printf("项目运行的开始时间：%f\n",startTime);
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
	//glfwSetCursorPosCallback(window, mouse_callback);
	//glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

		ourShader.Use();

		// Transformation matrices
		glm::mat4 projection = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.getViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "view"), 1, GL_FALSE, glm::value_ptr(view));

		// Draw the loaded model
		glm::mat4 model;
		//model = glm::rotate(model, glm::radians(glfwGetTime()) * 100, glm::vec3(0.0f, 1.0f, 0.0f));
		
		model = glm::rotate(model, glm::radians(angleH), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(angleV), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // Translate it down a bit so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// It's a bit too big for our scene, so scale it down
		glUniformMatrix4fv(glGetUniformLocation(ourShader.program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		ourModel.Draw(ourShader);
		if (flag == 1)
		{
			endTime = glfwGetTime();
			printf("项目结束时间：%f\n", endTime);
			loadTime = endTime - startTime;
			printf("加载model消耗的时间：%f\n", loadTime);
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
		
	if (keys[GLFW_KEY_A])
		//camera.ProcessKeyBoard(LEFT, deltaTime);
	{
		angleH += reduce;
		//camera.Yaw += reduce;
	}
	if (keys[GLFW_KEY_D])
		//camera.ProcessKeyBoard(RIGHT, deltaTime);
	{
		angleH -= reduce;
		//camera.Yaw -= reduce;
	}
	

	if (keys[GLFW_KEY_Q]){
		//camera.Pitch -= reduce;
		angleV += reduce;
	}
	if (keys[GLFW_KEY_E]){
		//camera.Pitch += reduce;
		angleV -= reduce;
	}
	/*
	if (camera.Pitch >= 89.0f)
	{
		camera.Pitch = 89.0f;
	}
	if (camera.Pitch <= -89.0f)
	{
		camera.Pitch = -89.0f;
	}
	*/
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

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

#pragma endregion