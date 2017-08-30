#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "BTGLHandle.h"
#include "stdImage\stb_image.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();

float alpha = 0.0f;
float rotateAngle = 0.0f;
enum {triangle = 1 , ball = 2};
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -3.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

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
	handle->init(triangle);
	while (!glfwWindowShouldClose(window))
	{
		//printf("time:%f\n", (GLfloat)glfwGetTime());
		//��鴥���¼�
		//�������롢����ƶ���
		glfwPollEvents();
		do_movement();
		//��ת�Ƕ�
		rotateAngle += 0.01f;
		//������
		handle->drawTriangles(triangle, alpha, rotateAngle, cameraPos, cameraFront, cameraUp);
		//˫���漼��,��������
		glfwSwapBuffers(window);
	}
	//�ͷ���Դ
	glfwTerminate();
	
	return 0;
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
		if (cameraPos.z >= 13.0f)
		{
			cameraPos.z = 13.0f;
		}
		else{
			cameraPos -= cameraSpeed * cameraFront;
		}
	}
	if (keys[GLFW_KEY_A])
	{
		if (cameraPos.x <= -3.0f)
		{
			cameraPos.x = -3.0f;
		}
		else{
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}	
	}
	if (keys[GLFW_KEY_D])
	{
		if (cameraPos.x >= 3.0f)
		{
			cameraPos.x = 3.0f;
		}
		else{
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		}
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

