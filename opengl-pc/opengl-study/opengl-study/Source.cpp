#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"
#include "stdImage\stb_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
void mouse_callback(GLFWwindow *window , double xpos , double ypos);

float alpha = 0.0f;
float rotateAngle = 0.0f;
enum {triangle = 1 , ball = 2};
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//������ ƫ����
GLfloat pitch = 0.0f;
GLfloat yaw = -90.0f;

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
		//��ת�Ƕ�
		rotateAngle += 0.01f;
		glm::vec3 front; 
		front.y = sin(glm::radians(pitch));
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
		//������
		handle->drawTriangles(triangle, alpha, rotateAngle, cameraPos, cameraFront, cameraUp);
		//˫���漼��,��������
		glfwSwapBuffers(window);
	}
	//�ͷ���Դ
	glfwTerminate();
	
	return 0;
}

//�������Ĭ��λ��Ϊ��������
GLfloat lastX = 400;
GLfloat lastY = 300;

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	GLfloat xoffset = xpos - lastX;
	//y����Ϊ�� ����Ϊ-  ������ϵ�෴  �������ⷴ��
	GLfloat yoffset = lastY - ypos;
	printf("xoffset : %f , yoffset:%f\n",xoffset , yoffset);
	lastX = xpos;
	lastY = ypos;
	//����һ�������ȣ���Ȼ����ƶ�̫��
	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	pitch += yoffset;
	yaw += xoffset;
	printf("pitch:%f , yaw:%f\n",pitch , yaw);
	//�������ƶ���Χ -89�� ~ 89��
	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}
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
	//Ϊ�˱�֤�ڲ�ͬ�����ϣ�����������ƶ��ٶ�һ��d
	GLfloat cameraSpeed = 5.0f * deltaTime;
	//���������
	if (keys[GLFW_KEY_W])
	{

		cameraPos += cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_S])
	{
			cameraPos -= cameraSpeed * cameraFront;
	}
	if (keys[GLFW_KEY_A])
	{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (keys[GLFW_KEY_D])
	{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
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
		printf("alpha:%f\n", alpha);
		//if (alpha >= 1.0f)
		//{
		//	alpha = 1.0f;
		//}
	}
	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		alpha -= 1.0f;
		printf("alpha:%f\n", alpha);
		//if (alpha <= 0.0f)
		//{
		//	alpha = 0.0f;
		//}
	}
};

