#include <iostream>
#include <math.h>

//GLEW
#include <GL/glew.h>
//GLFW
#include <GLFW/glfw3.h>
//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
//inner
#include "BTShader.h"
#include "utils.h"
#include "stb_image.h"
#include "BTCamera.h"
#include "time_util.h"

GLfloat screen_width = 800.0f, screen_height = 600.0f;
GLuint vao, vbo , pbo , textureId;
GLfloat vertexs[] = {
	/*
	3,2
	0,1
	*/
// 	-0.5, -0.5,
// 	0.5, -0.5,
// 	0.5, 0.5,
// 	-0.5, 0.5

	/*
	2,3
	0,1
	*/
	-0.5, -0.8,
	0.5, -0.8,
	0.3, 0.6,
	-0.3, 0.6
};
GLfloat texVerts[] = 
{
	-0.5, -0.5, 0,0,
	0.5, -0.5, 1,0,
	0.5, 0.5,1,1,
	-0.5, 0.5,0,1
};
enum Attr_id
{
	vPosition = 1,
	vTextureCoords = 2
};
//texture
unsigned char* imageData;
int width = 0, height = 0, channels = 0;
Camera *camera = new Camera(glm::vec3(0.0f, 0.0f, 5.0f));
//key、mouse
GLfloat lastX = 0.0f;
GLfloat lastY = 0.0f;
GLfloat deltaTime = 0.0f;//当前帧遇上上一帧的时间增量
GLfloat lastFrame = 0.0f;//上一帧的时间
GLboolean firstMouse = true;
GLboolean keys[1024];
GLFWwindow *window[2] = {0};
const char *titles[2] = {"aaa","bbb"};
GLuint fbo = 0;
GLuint fboTex = 0;
GLuint vaoTex = 0;
GLuint vboTex = 0;

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);//键盘输入
void mouse_callback(GLFWwindow *window, double xpos, double ypos);//鼠标移动
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);//鼠标滚轮缩放大小
void do_movement();
void initVAO();

void dump_texture(GLuint texture_id, int *pWidth, int *pHeight)
{
	//glGetIntegerv(GL_TEXTURE_BINDING_2D, &curTex);
	glBindTexture(GL_TEXTURE_2D, texture_id);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, pWidth);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, pHeight);

	uint8_t *pix = (uint8_t*)malloc(*pWidth * *pHeight * 4);
	char buff[128];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, pix);

	sprintf(buff, "d:/code/work/dump_%dx%d.rgba", *pWidth, *pHeight);
	FILE *fp = fopen(buff, "wb");
	fwrite(pix, 1, *pWidth * *pHeight * 4, fp);
	fclose(fp);
	free(pix);

	glBindTexture(GL_TEXTURE_2D, 0);

}

void main(){
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);//opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);//use opengl core

	for (int i = 0 ; i < 1 ; i++)
	{
		bool isFullScreen = false;
		int monitorCount;
		GLFWmonitor** pMonitor = isFullScreen ? glfwGetMonitors(&monitorCount) : NULL;

		//window[i] = glfwCreateWindow((int)screen_width, (int)screen_height, titles[i], pMonitor[i], NULL);
		window[i] = glfwCreateWindow((int)screen_width, (int)screen_height, titles[i], NULL, NULL);
		if (window[i] == NULL)//check window
		{
			printf("create GLFW window failed");
			glfwTerminate();
		}
		//glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window[i], key_callback);//keyboard
		//glfwSetInputMode(window[i], GLFW_CURSOR, GLFW_CURSOR_DISABLED);//hind cursor
		glfwSetCursorPosCallback(window[i], mouse_callback);//mouse move
		glfwSetScrollCallback(window[i], scroll_callback);//mouse roller
	}

	//init glew
	glfwMakeContextCurrent(window[0]);
	glewExperimental = true;
	glewInit(); 

	glfwMakeContextCurrent(window[0]);

	initVAO();
	BTShader shader("vertex_shader.vert", "fragment_shader.frag");
	BTShader shaderTex("vs.vert", "fs.frag");

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenTextures(1, &fboTex);
	glBindTexture(GL_TEXTURE_2D, fboTex);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screen_width, screen_height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//loop
	while (!glfwWindowShouldClose(window[0]))
	{
		for (int i = 0 ; i < 1 ; i++)
		{
			glfwPollEvents();//检查触发事件
			do_movement();//keyboard

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(0.2f, 0.2f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height);
			shader.Use();
// 			glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);//投影矩阵
// 			glm::mat4 viewMatrix = camera->getViewMatrix();//视图矩阵
// 			glUniformMatrix4fv(glGetUniformLocation(shader.program, "view"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
// 			glUniformMatrix4fv(glGetUniformLocation(shader.program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			//draw
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);

			int ww, hh;
			dump_texture(fboTex, &ww, &hh);

// 			glBindFramebuffer(GL_FRAMEBUFFER, 0);
// 			glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
// 			glClear(GL_COLOR_BUFFER_BIT);
// 			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 			glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height);
// 			shaderTex.Use();
// 			glBindTexture(GL_TEXTURE_2D, fboTex);
// 			glBindVertexArray(vaoTex);
// 			glDrawArrays(GL_QUADS, 0, 4);
			// Swap the buffers
			glfwSwapBuffers(window[i]);
		}

	}

	glfwMakeContextCurrent(NULL);

}

void initVAO(){
	//vao
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glBindVertexArray(vao);
	GLboolean vao_light_b = glIsVertexArray(vao);
	//printf("vao bind status:%d\n", vao_light_b);
	//vbo
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//check VBO bind status
	GLboolean vbo_b = glIsBuffer(vbo);
	//printf("vbo bind status:%d\n", vbo_b);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexs),vertexs,GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE,sizeof(GLfloat) * 2 , 0);
	//glVertexAttribPointer(vTextureCoords,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat) * 5 , (const void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(vPosition);
	//glEnableVertexAttribArray(vTextureCoords);
	glBindVertexArray(0);

	glGenVertexArrays(1, &vaoTex);
	glGenBuffers(1, &vboTex);
	glBindVertexArray(vaoTex);
	glBindBuffer(GL_ARRAY_BUFFER, vboTex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(texVerts), texVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 4, 0);
	glVertexAttribPointer(2, 2,GL_FLOAT,GL_FALSE,sizeof(GLfloat) * 4 , (const void*)(sizeof(GLfloat) * 2));
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);
}
//键盘鼠标
void mouse_callback(GLFWwindow *window, double xpos, double ypos){
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	//y向下为增 向上为-  与坐标系相反  所以在这反减
	GLfloat yoffset = lastY - ypos;
	//printf("xoffset : %f , yoffset:%f\n",xoffset , yoffset);
	lastX = xpos;
	lastY = ypos;
	camera->ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset){
	camera->ProcessMouseScroll(yoffset);
}
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
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
void do_movement(){
	GLfloat currentTime = glfwGetTime();
	deltaTime = currentTime - lastFrame;
	lastFrame = currentTime;
	//摄像机控制
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