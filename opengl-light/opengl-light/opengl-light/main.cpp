#include <iostream>
//glew
#define GLEW_STATIC
#include <../GL/glew.h>
//glfw
#include <../GLFW/glfw3.h>

#include "BTGLHandle.h"
#include "BTCamera.h"

//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

Camera *camera = new Camera();

void main(){
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GL_TRUE);
	glEnable(GL_DEBUG_OUTPUT);
	GLFWwindow *window = glfwCreateWindow(800, 600, "OpenGL-Light", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "glfw init failed" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	//init glew
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "glew init failed" << std::endl;
	}
	//start
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0,0,width,height);

	Handle *handle = new Handle();
	handle->init(handle->TRIANGLE);
	while (!glfwWindowShouldClose(window))
	{
		//检查触发事件
		glfwPollEvents();
		//键盘输入
		do_movement();
		//entrace
		glm::mat4 viewMatrix = camera->getViewMatrix();
		handle->draw(handle->TRIANGLE,viewMatrix);
		//双缓存技术,交换缓冲
		glfwSwapBuffers(window);
	}
	//end
	glfwTerminate();
}

GLboolean keys[1024];
//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "account press ESC" << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
};

//当前帧遇上上一帧的时间增量
GLfloat deltaTime = 0.0f;
//上一帧的时间
GLfloat lastFrame = 0.0f;

void do_movement(){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	//摄像机控制
	if (keys[GLFW_KEY_W])
	{
		camera->ProcessKeyBoard(FORWARD, deltaTime);
	}
	if (keys[GLFW_KEY_S])
	{
		camera->ProcessKeyBoard(BACKWARD, deltaTime);
	}
	if (keys[GLFW_KEY_A])
	{
		camera->ProcessKeyBoard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera->ProcessKeyBoard(RIGHT, deltaTime);
	}
}
