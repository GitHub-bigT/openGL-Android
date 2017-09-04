#include "BTGL.h"

void BTBind::init(Shape shape){
	if (shape == this->TRIANGLE)
	{
		this->initTriangle();
	}
}

void BTBind::draw(Shape shape, GLuint programId){
	if (shape == this->TRIANGLE)
	{
		this->drawTriangle(programId);
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

void BTBind::drawTriangle(GLuint programId){
	//glEnable(GL_DEPTH_TEST);
	
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[TriangleVAO]);
	
	glDrawArrays(GL_TRIANGLES,0,3);
	
}