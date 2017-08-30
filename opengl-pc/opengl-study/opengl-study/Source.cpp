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
	handle->init(triangle);
	while (!glfwWindowShouldClose(window))
	{
		//printf("time:%f\n", (GLfloat)glfwGetTime());
		//检查触发事件
		//键盘输入、鼠标移动等
		glfwPollEvents();
		do_movement();
		//旋转角度
		rotateAngle += 0.01f;
		//三角形
		handle->drawTriangles(triangle, alpha, rotateAngle, cameraPos, cameraFront, cameraUp);
		//双缓存技术,交换缓冲
		glfwSwapBuffers(window);
	}
	//释放资源
	glfwTerminate();
	
	return 0;
}

//存储按键状态的数组
bool keys[1024];
//当前帧遇上上一帧的时间增量
GLfloat deltaTime = 0.0f;
//上一帧的时间
GLfloat lastFrame = 0.0f;
//
void do_movement(){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	//为了保证在不同机器上，保持摄像机移动速度一致d
	GLfloat cameraSpeed = 5.0f * deltaTime;
	//摄像机控制
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

