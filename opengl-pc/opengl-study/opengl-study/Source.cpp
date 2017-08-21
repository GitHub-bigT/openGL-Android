#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(){
	glfwInit();
	//设置opengl的主版本和次版本（3.3）
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//设置窗口
	GLFWwindow* window = glfwCreateWindow(800,600,"bigT-OpenGL",nullptr,nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		//释放资源
		glfwTerminate();
		return -1;
	}
	//通知GLFW将窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);
	//
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "failed to initialize GLEW" << std::endl;
		return -1;
	}
	//获取glfw窗口的width和height
	int width, height;
	glfwGetFramebufferSize(window,&width,&height);
	std::cout << "width:" << width << "  height:" << height << std::endl;
	glViewport(0,0,width,height);
	//设置键盘监听事件
	glfwSetKeyCallback(window,key_callback);
	//渲染三角形
	Handle* handle = new Handle();
	while (!glfwWindowShouldClose(window))
	{
		//检查触发事件
		//键盘输入、鼠标移动等
		glfwPollEvents();
		//渲染指令
		glClearColor(0.3f,0.6f,0.7f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//三角形
		handle->drawTriangles();
		//双缓存技术,交换缓冲
		glfwSwapBuffers(window);
	}
	//释放资源
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key , int scancode , int action , int mode){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "account press ESC" << std::endl;
		glfwSetWindowShouldClose(window,GL_TRUE);
	}
};

