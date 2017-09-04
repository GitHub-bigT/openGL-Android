#include "BTGL.h"

void BTBind::init(Shape shape){
	if (shape == this->TRIANGLE)
	{
		this->initTriangle();
	}
}

void BTBind::draw(Shape shape, GLuint programId, glm::mat4 viewMatrix){
	if (shape == this->TRIANGLE)
	{
		this->drawTriangle(programId,viewMatrix);
	}
}

void BTBind::initTriangle(){

	BTError *error = new BTError();

	//vao
	glGenVertexArrays(NumVAOIds,VAOs);
	
	glBindVertexArray(VAOs[TriangleVAO]);
	
	//check VAO bind status
	GLboolean vao_b = glIsVertexArray(VAOs[TriangleVAO]);
	printf("vao bind status:%d\n",vao_b);
	//vbo
	glGenBuffers(NumVBOIds,VBOs);
	
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	
	//check VBO bind status
	GLboolean vbo_b = glIsBuffer(VBOs[TriangleVBO]);
	printf("vbo bind status:%d\n",vbo_b);
	GLfloat triangleVertexs[] = {
		0.0f, 1.0f, 0.0f,
		-0.5f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f
	};
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertexs), triangleVertexs, GL_STATIC_DRAW);

	glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,sizeof(GLfloat) * 3 , 0);
	error->glCheckError();
	glEnableVertexAttribArray(this->vPosition);
	glBindVertexArray(0);
	
}

void BTBind::drawTriangle(GLuint programId, glm::mat4 viewMatrix){
	//glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[TriangleVAO]);

	GLfloat currentTime = glfwGetTime();

	//模型矩阵
	glm::mat4 model;
	//model = glm::translate(model,glm::vec3(0.0f,0.0f,-3.0f));
	model = glm::rotate(model, glm::radians(currentTime) * 100, glm::vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(glGetUniformLocation(programId,"model"),1,GL_FALSE,glm::value_ptr(model));
	//视图矩阵
	glUniformMatrix4fv(glGetUniformLocation(programId,"view"),1,GL_FALSE,glm::value_ptr(viewMatrix));
	//投影矩阵
	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	glDrawArrays(GL_TRIANGLES,0,3);
	
}