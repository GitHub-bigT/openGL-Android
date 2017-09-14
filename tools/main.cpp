#include <iostream>

//GLEW
#include <../GL/glew.h>
//GLFW
#include <../GLFW/glfw3.h>
//GLM
#include <../glm/glm.hpp>
#include <../glm/gtc/matrix_transform.hpp>
#include <../glm/gtc/type_ptr.hpp>
//inner
#include "BTShader.h"

GLfloat screen_width = 800.0f, screen_height = 600.0f;
GLuint vao, vbo;
GLfloat vertexs[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);//��������
void initVAO();

void main(){
	//init glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);//opengl 4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);//use opengl core
	GLFWwindow *window = glfwCreateWindow(screen_width,screen_height,"opengl-pr",NULL,NULL);
	if (window == NULL)//check window
	{
		printf("create GLFW window failed");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	//init glew
	glewExperimental = GL_TRUE;
	glewInit();

	//init opengl
	glViewport(0,0,screen_width,screen_height);
	initVAO();
	BTShader shader("vertex_shader.vert","fragment_shader.frag");
	shader.Use();

	//loop
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//��鴥���¼�
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

//��������
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode){

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
};
enum Attr_id
{
	vPosition = 1
};
void initVAO(){
	
	
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);

	glBindVertexArray(vao);

	//check VAO bind status
	GLboolean vao_light_b = glIsVertexArray(vao);
	printf("vao bind status:%d\n", vao_light_b);

	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	//check VBO bind status
	GLboolean vbo_b = glIsBuffer(vbo);
	printf("vbo bind status:%d\n", vbo_b);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexs),vertexs,GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(vPosition);
	glBindVertexArray(0);
}