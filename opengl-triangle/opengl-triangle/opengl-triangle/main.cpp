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

GLfloat screen_width = 800.0f, screen_height = 600.0f;
GLuint vao, vbo , pbo , textureId;
GLfloat vertexs[] = {
	0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	0.5f, -0.5f, 0.0f, 1.0f, 0.0f
};
//texture
unsigned char* imageData;
int width = 0, height = 0, channels = 0;

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);//键盘输入
void initVAO();

void main(){
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);//opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//use opengl core
	GLFWwindow *window = glfwCreateWindow((int)screen_width,(int)screen_height,"opengl-pr",NULL,NULL);
	if (window == NULL)//check window
	{
		printf("create GLFW window failed");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//init glew
	glewExperimental = true;
	glewInit(); 

	//init opengl
	glViewport(0, 0, (GLsizei)screen_width, (GLsizei)screen_height);
	initVAO();
	BTShader shader("vertex_shader.vert","fragment_shader.frag");
	shader.Use();

	//loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//检查触发事件
		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,3);
		// Swap the buffers
		glfwSwapBuffers(window);
	}
}

//键盘输入
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
};
enum Attr_id
{
	vPosition = 1,
	vTextureCoords = 2
};
void initVAO(){
	//vao
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
	glBindVertexArray(vao);
	GLboolean vao_light_b = glIsVertexArray(vao);
	printf("vao bind status:%d\n", vao_light_b);
	//vbo
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//check VBO bind status
	GLboolean vbo_b = glIsBuffer(vbo);
	printf("vbo bind status:%d\n", vbo_b);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexs),vertexs,GL_STATIC_DRAW);
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE,sizeof(GLfloat) * 5 , 0);
	glVertexAttribPointer(vTextureCoords,2,GL_FLOAT,GL_FALSE,sizeof(GLfloat) * 5 , (const void*)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vTextureCoords);
	glBindVertexArray(0);
}
