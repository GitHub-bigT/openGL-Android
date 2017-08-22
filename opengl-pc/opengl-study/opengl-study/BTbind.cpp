#include "BTGL.h"
void BTVaoVbo::initVaoVbo(){
	//std::cout << "init vao vbo" << std::endl;
	GLfloat triangleVertex[6] = {
			0.0f, 0.5f,
			-0.5f, -0.5f,
			0.5f, -0.5f
	};
	GLfloat triangleColor[12] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f
	};
	//1.��VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleBO]);
	//2.�����ݴ����Դ浱�е�ARRAY_BUFFER�������
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertex)+sizeof(triangleColor),NULL,GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangleVertex),triangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangleVertex), sizeof(triangleColor), triangleColor);

	GLboolean b = glIsBuffer(VBOs[TriangleBO]);
	printf("triangle vbo bind: %d\n", b);
	//(ig75icd32.dll) ����
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//1.��VAO
	glGenVertexArrays(NumVaoIds, VAOs);
	glBindVertexArray(VAOs[TriangleAO]);
	GLboolean b1 = glIsVertexArray(VAOs[TriangleAO]);
	printf("triangle vao bind: %d\n", b1);
	//2.���ö�������ָ��
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(triangleVertex));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
	//3.���vao,�����������ط����������
	glBindVertexArray(0);
}

void BTVaoVbo::drawArrays(){
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[TriangleAO]);
	glDrawArrays(GL_TRIANGLE_STRIP,0,3);
	glBindVertexArray(0);
	glFlush();
}