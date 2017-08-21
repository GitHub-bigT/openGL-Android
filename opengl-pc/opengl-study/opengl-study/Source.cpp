#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(){
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
	glewExperimental = GL_TRUE;
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
	while (!glfwWindowShouldClose(window))
	{
		//��鴥���¼�
		//�������롢����ƶ���
		glfwPollEvents();
		//��Ⱦָ��
		glClearColor(0.3f,0.6f,0.7f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//������
		handle->drawTriangles();
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
};

