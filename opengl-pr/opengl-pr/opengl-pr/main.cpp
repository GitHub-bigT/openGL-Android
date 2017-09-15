#include <iostream>
#include <math.h>

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
GLfloat vertexs1[] = {
	0.0f, 0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f
};
float vertexs[58008];

//function
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);//键盘输入
void initVAO();
void generateDome(float radius, int sectors, float degreeY, bool isUpper);

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
	generateDome(18.0f,150,180.0f,true);
	initVAO();
	BTShader shader("vertex_shader.vert","fragment_shader.frag");
	shader.Use();

	//loop
	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();//检查触发事件
		// Clear the colorbuffer
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//draw
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES,0,50);
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

 void generateDome(float radius, int sectors, float degreeY, bool isUpper) {


	float percent = degreeY / 360;
	int rings = sectors >> 1;

	printf("percent:%f,rings:%d\n", percent, rings);


	float R = 1.0f / rings;
	float S = 1.0f / sectors;
	short r, s;
	float x, y, z;

	int lenRings = (int)(rings * percent) + 1;
	int lenSectors = sectors + 1;
	int numPoint = lenRings * lenSectors;

	//printf("percent:%f,rings:%d\n", percent, rings);
	//printf("R:%f,S:%f\n", R, S);
	//printf("lenRings:%d,lenSectors:%d,numPoint:%d\n", lenRings, lenSectors, numPoint);
	printf("numPoint:%d\n", numPoint);
	

	float PI = 3.1415926;
	float PI_2 = PI / 2.0f;

	int upper = isUpper ? 1 : -1;

	int t = 0, v = 0;
	for (r = 0; r < lenRings; r++) {
		for (s = 0; s < lenSectors; s++) {
			x = (float)(glm::cos(glm::radians(2 * PI * s * S)) * glm::sin(glm::radians(PI * r * R))) * upper;
			y = (float)glm::sin(glm::radians(-PI_2 + PI * r * R)) * -upper;
			z = (float)(glm::sin(glm::radians(2 * PI * s * S)) * glm::sin(glm::radians(PI * r * R)));

			//float a = (float)(Math.cos(2 * PI * s * S) * r * R / percent) / 2.0f + 0.5f;
			//float b = (float)(Math.sin(2 * PI * s * S) * r * R / percent) / 2.0f + 0.5f;

			//texcoords[t++] = b;
			//texcoords[t++] = a;

			vertexs[v++] = x * radius;
			vertexs[v++] = y * radius;
			vertexs[v++] = z * radius;
		}
	}

	/*

	int counter = 0;
	for (r = 0; r < lenRings - 1; r++){
		for (s = 0; s < lenSectors - 1; s++) {
			indices[counter++] = (short)(r * lenSectors + s);       //(a)
			indices[counter++] = (short)((r + 1) * lenSectors + (s));    //(b)
			indices[counter++] = (short)((r)* lenSectors + (s + 1));  // (c)
			indices[counter++] = (short)((r)* lenSectors + (s + 1));  // (c)
			indices[counter++] = (short)((r + 1) * lenSectors + (s));    //(b)
			indices[counter++] = (short)((r + 1) * lenSectors + (s + 1));  // (d)
		}
	}

	// initialize vertex byte buffer for shape coordinates
	ByteBuffer bb = ByteBuffer.allocateDirect(
		// (# of coordinate values * 4 bytes per float)
		vertexs.length * 4);
	bb.order(ByteOrder.nativeOrder());
	FloatBuffer vertexBuffer = bb.asFloatBuffer();
	vertexBuffer.put(vertexs);
	vertexBuffer.position(0);

	// initialize vertex byte buffer for shape coordinates
	ByteBuffer cc = ByteBuffer.allocateDirect(
		texcoords.length * 4);
	cc.order(ByteOrder.nativeOrder());
	FloatBuffer texBuffer = cc.asFloatBuffer();
	texBuffer.put(texcoords);
	texBuffer.position(0);

	// initialize byte buffer for the draw list
	ByteBuffer dlb = ByteBuffer.allocateDirect(
		// (# of coordinate values * 2 bytes per short)
		indices.length * 2);
	dlb.order(ByteOrder.nativeOrder());
	ShortBuffer indexBuffer = dlb.asShortBuffer();
	indexBuffer.put(indices);
	indexBuffer.position(0);
	*/
}