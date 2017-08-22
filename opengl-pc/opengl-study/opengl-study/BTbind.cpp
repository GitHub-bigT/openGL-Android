#include "BTGL.h"
void BTVaoVbo::initVaoVbo(){
	//std::cout << "init vao vbo" << std::endl;
	GLfloat triangleVertex[8] = {
			0.5f, 0.5f, //右上
			0.5f, -0.5f, //右下
			-0.5f, -0.5f, //左下
			- 0.5f, 0.5f, //左上
	};
	GLushort triangleIndex[6] = {
			0,1,3,
			1,2,3
	};
	GLfloat triangleColor[32] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f
	};
	

	//1.绑定VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//2.将数据传给显存当中的ARRAY_BUFFER缓冲块中
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertex)+sizeof(triangleColor),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangleVertex),triangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangleVertex), sizeof(triangleColor), triangleColor);
	GLboolean b = glIsBuffer(VBOs[TriangleVBO]);
	printf("triangle vbo bind: %d\n", b);
	//(ig75icd32.dll) 崩溃
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	//1.绑定VAO
	glGenVertexArrays(NumVaoIds, VAOs);
	glBindVertexArray(VAOs[TriangleVAO]);
	GLboolean b1 = glIsVertexArray(VAOs[TriangleVAO]);
	printf("triangle vao bind: %d\n", b1);
	//1.绑定EBO or IBO
	glGenBuffers(NumEBOIds , EBOs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBOs[TriangleEBO]);
	GLboolean b2 = glIsBuffer(EBOs[TriangleEBO]);
	printf("triangle ebo bind: %d\n", b2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triangleIndex),triangleIndex,GL_STATIC_DRAW);


	
	//2.设置顶点属性指针
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(triangleVertex));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
	//3.解绑vao,避免在其他地方错误的配置
	glBindVertexArray(0);
}

void BTVaoVbo::drawArrays(){
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(VAOs[TriangleVAO]);
	//GL_FILL 默认模式  
	//GL_LINE 线框模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
	glBindVertexArray(0);
	
	glFlush();
}