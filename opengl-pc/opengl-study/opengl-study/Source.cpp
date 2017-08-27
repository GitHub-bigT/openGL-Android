#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"
#include "stdImage\stb_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

float alpha = 0.5f;
float rotateAngle = 0.0f;
enum {triangle = 1 , ball = 2};

int main(){
	/*
	int width1, height1, nrChannels1;
	unsigned char* image1 = stbi_load("./sImage/wx4.jpg", &width1, &height1, &nrChannels1, 0);
	printf("image : %s\n", image1);
	printf("image : -=================================\n");
	int i;
	std::cin>>i;
	*/
	
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
	glewExperimental = true;
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
	handle->init(ball);
	while (!glfwWindowShouldClose(window))
	{
		//检查触发事件
		//键盘输入、鼠标移动等
		glfwPollEvents();
		//旋转角度
		rotateAngle += 0.1f;
		//三角形
		handle->drawTriangles(ball, alpha, rotateAngle);
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
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		alpha += 0.1f;
		if (alpha >= 1.0f)
		{
			alpha = 1.0f;
		}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		alpha -= 0.1f;
		if (alpha <= 0.0f)
		{
			alpha = 0.0f;
		}
	}
	printf("alpha ----- : %f\n", alpha);
};

