#include <iostream>
//glew
#define GLEW_STATIC
#include <../GL/glew.h>
//glfw
#include <../GLFW/glfw3.h>

#include "BTGLHandle.h"
#include "BTCamera.h"

//��������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
//����ƶ�
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
//���������Ŵ�С
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

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
	//���ع��
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//����ƶ�������ŷ���ǣ�
	glfwSetCursorPosCallback(window, mouse_callback);
	//�������Ŵ�С
	glfwSetScrollCallback(window, scroll_callback);
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
		//��鴥���¼�
		glfwPollEvents();
		//��������
		do_movement();
		//entrace
		glm::mat4 viewMatrix = camera->getViewMatrix();
		handle->draw(handle->TRIANGLE,viewMatrix,camera);
		//˫���漼��,��������
		glfwSwapBuffers(window);
	}
	//end
	glfwTerminate();
}

GLboolean keys[1024];
//��������
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

//��ǰ֡������һ֡��ʱ������
GLfloat deltaTime = 0.0f;
//��һ֡��ʱ��
GLfloat lastFrame = 0.0f;

void do_movement(){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
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
		camera->ProcessKeyBoard(LEFT, deltaTime);
	}
	if (keys[GLFW_KEY_D])
	{
		camera->ProcessKeyBoard(RIGHT, deltaTime);
	}
}

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