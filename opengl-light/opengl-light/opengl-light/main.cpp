#include <iostream>
//glew
#define GLEW_STATIC
#include <../GL/glew.h>
//glfw
#include <../GLFW/glfw3.h>

#include "BTGLHandle.h"

//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

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
		//entrace
		handle->draw(handle->TRIANGLE);
		//双缓存技术,交换缓冲
		glfwSwapBuffers(window);
	}
	//end
	glfwTerminate();
}

//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "account press ESC" << std::endl;
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
};
