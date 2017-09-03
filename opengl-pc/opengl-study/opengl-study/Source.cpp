#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"
#include "stdImage\stb_image.h"
#include "Camera.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow *window , double xpos , double ypos);
void scroll_callback(GLFWwindow *window , double xoffset, double yoffset);

float alpha = 0.0f;
enum {triangle = 1 , ball = 2};

Camera *camera =  new Camera();

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
	handle->init(triangle);
	while (!glfwWindowShouldClose(window))
	{
		//��鴥���¼�
		glfwPollEvents();
		//��������
		do_movement();
		//���ع��
		glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
		//����ƶ�������ŷ���ǣ�
		glfwSetCursorPosCallback(window,mouse_callback);
		//�������Ŵ�С
		glfwSetScrollCallback(window, scroll_callback);

		glm::mat4 viewMatrix = camera->getViewMatrix();
		//������
		handle->drawTriangles(triangle, alpha, viewMatrix, camera->Zoom);
		//˫���漼��,��������
		glfwSwapBuffers(window);
	}
	//�ͷ���Դ
	glfwTerminate();
	
	return 0;
}

//�������Ĭ��λ��Ϊ��������
GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
GLboolean firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	//y����Ϊ�� ����Ϊ-  ������ϵ�෴  �������ⷴ��
	GLfloat yoffset = lastY - ypos;
	//printf("xoffset : %f , yoffset:%f\n",xoffset , yoffset);
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
	camera->ProcessMouseScroll(yoffset);
}

//�洢����״̬������
bool keys[1024];
//��ǰ֡������һ֡��ʱ������
GLfloat deltaTime = 0.0f;
//��һ֡��ʱ��
GLfloat lastFrame = 0.0f;
//
void do_movement(){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	//Ϊ�˱�֤�ڲ�ͬ�����ϣ�����������ƶ��ٶ�һ��
	GLfloat cameraSpeed = 5.0f * deltaTime;
	//���������
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
		camera->ProcessKeyBoard(LEFT , deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera->ProcessKeyBoard(RIGHT, deltaTime);
	}
}

void key_callback(GLFWwindow* window, int key , int scancode , int action , int mode){
	
	if (action == GLFW_PRESS)
	{
		keys[key] = true;
	}
	if (action == GLFW_RELEASE)
	{
		keys[key] = false;
	}
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		std::cout << "account press ESC" << std::endl;
		glfwSetWindowShouldClose(window,GL_TRUE);	
	}
	if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		alpha += 1.0f;
		//printf("alpha:%f\n", alpha);
		//if (alpha >= 1.0f)
		//{
		//	alpha = 1.0f;
		//}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		alpha -= 1.0f;
		//printf("alpha:%f\n", alpha);
		//if (alpha <= 0.0f)
		//{
		//	alpha = 0.0f;
		//}
	}
};

