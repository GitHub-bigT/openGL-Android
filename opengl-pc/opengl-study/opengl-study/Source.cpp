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
	//����opengl�����汾�ʹΰ汾��3.3��
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//���ô���
	GLFWwindow* window = glfwCreateWindow(800,600,"bigT-OpenGL",nullptr,nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to create GLFW window" << std::endl;
		//�ͷ���Դ
		glfwTerminate();
		return -1;
	}
	//֪ͨGLFW�����ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);
	//
	glewExperimental = true;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "failed to initialize GLEW" << std::endl;
		return -1;
	}
	//��ȡglfw���ڵ�width��height
	int width, height;
	glfwGetFramebufferSize(window,&width,&height);
	std::cout << "width:" << width << "  height:" << height << std::endl;
	glViewport(0,0,width,height);
	//���ü��̼����¼�
	glfwSetKeyCallback(window,key_callback);
	//��Ⱦ������
	Handle* handle = new Handle();
	handle->init(ball);
	while (!glfwWindowShouldClose(window))
	{
		//��鴥���¼�
		//�������롢����ƶ���
		glfwPollEvents();
		//��ת�Ƕ�
		rotateAngle += 0.1f;
		//������
		handle->drawTriangles(ball, alpha, rotateAngle);
		//˫���漼��,��������
		glfwSwapBuffers(window);
	}
	//�ͷ���Դ
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

