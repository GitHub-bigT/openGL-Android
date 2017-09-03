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
//fov
//滚轮向下滑动 ++  向上--
//默认为最大视野
GLfloat aspect = 45.0f;

Camera *camera =  new Camera();

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
		//检查触发事件
		glfwPollEvents();
		//键盘输入
		do_movement();
		//隐藏光标
		glfwSetInputMode(window,GLFW_CURSOR,GLFW_CURSOR_DISABLED);
		//鼠标移动（采用欧拉角）
		glfwSetCursorPosCallback(window,mouse_callback);
		//滚轮缩放大小
		glfwSetScrollCallback(window, scroll_callback);

		glm::mat4 viewMatrix = camera->getViewMatrix();
		//三角形
		handle->drawTriangles(triangle, alpha, viewMatrix, aspect);
		//双缓存技术,交换缓冲
		glfwSwapBuffers(window);
	}
	//释放资源
	glfwTerminate();
	
	return 0;
}

//设置鼠标默认位置为窗口中心
GLfloat lastX = 400;
GLfloat lastY = 300;

void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	GLfloat xoffset = xpos - lastX;
	//y向下为增 向上为-  与坐标系相反  所以在这反减
	GLfloat yoffset = lastY - ypos;
	//printf("xoffset : %f , yoffset:%f\n",xoffset , yoffset);
	lastX = xpos;
	lastY = ypos;
	
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
	if (aspect >= 1.0f && aspect <= 45.0f)
	{
		aspect -= yoffset;
	}
	if (aspect <= 1.0f)
	{
		aspect = 1.0f;
	}
	if (aspect >= 45.0f)
	{
		aspect = 45.0f;
	}
	//printf("aspect:%f\n",aspect);
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
	//为了保证在不同机器上，保持摄像机移动速度一致
	GLfloat cameraSpeed = 5.0f * deltaTime;
	//摄像机控制
	if (keys[GLFW_KEY_W])
	{


	}
	if (keys[GLFW_KEY_S])
	{

	}
	if (keys[GLFW_KEY_A])
	{

	}
	if (keys[GLFW_KEY_D])
	{

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

