#include "BTGL.h"
void BTVaoVbo::initVaoVbo(){
	//std::cout << "init vao vbo" << std::endl;
	GLfloat triangleVertex[8] = {
		0.5f, 0.5f, //����
		0.5f, -0.5f, //����
		-0.5f, -0.5f, //����
		-0.5f, 0.5f, //����
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
	GLfloat triangleTexture[8] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};
	
	//ͼ1 ����
	int width1, height1;
	unsigned char* image1 = SOIL_load_image("./sImage/container.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);
	glGenTextures(NumTexIds, TEXs);
	glBindTexture(GL_TEXTURE_2D, TEXs[Pic1]);
	//���������Ʒ�ʽ
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//�����������
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ����ڴ桢�������
	SOIL_free_image_data(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	//ͼ2 ����
	int width2, height2;
	unsigned char* image2 = SOIL_load_image("./sImage/png1.png", &width2, &height2, 0 ,SOIL_LOAD_RGBA);
	//unsigned char* image2 = SOIL_load_image("./sImage/biaoqing3.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);
	//printf("image2  ==== :%s\n",image2);
	glBindTexture(GL_TEXTURE_2D,TEXs[Pic2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ���ڴ桢�������
	SOIL_free_image_data(image2);
	glBindTexture(GL_TEXTURE_2D,0);

	//1.��VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//2.�����ݴ����Դ浱�е�ARRAY_BUFFER�������
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertex)+sizeof(triangleColor)+sizeof(triangleTexture),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangleVertex),triangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangleVertex), sizeof(triangleColor), triangleColor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(triangleVertex) + sizeof(triangleColor), sizeof(triangleTexture), triangleTexture);
	GLboolean b = glIsBuffer(VBOs[TriangleVBO]);
	printf("triangle vbo bind: %d\n", b);
	//(ig75icd32.dll) ����
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//1.��VAO
	glGenVertexArrays(NumVaoIds, VAOs);
	glBindVertexArray(VAOs[TriangleVAO]);
	GLboolean b1 = glIsVertexArray(VAOs[TriangleVAO]);
	printf("triangle vao bind: %d\n", b1);
	//1.��EBO or IBO 
	glGenBuffers(NumEBOIds , EBOs);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBOs[TriangleEBO]);
	GLboolean b2 = glIsBuffer(EBOs[TriangleEBO]);
	printf("triangle ebo bind: %d\n", b2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triangleIndex),triangleIndex,GL_STATIC_DRAW);

	//2.���ö�������ָ��
	glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(triangleVertex));
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(sizeof(triangleVertex)+sizeof(triangleColor)));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
	glEnableVertexAttribArray(vTexCoord);
	//3.���vao,�����������ط����������
	glBindVertexArray(0);
}

void BTVaoVbo::drawArrays(GLuint programId){
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(VAOs[TriangleVAO]);	
	//GL_FILL Ĭ��ģʽ  
	//GL_LINE �߿�ģʽ
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawArrays(GL_TRIANGLE_STRIP,0,4);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,0);
	glBindVertexArray(0);
	
	glFlush();
}

void BTVaoVbo::initSampler(GLuint programId){
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, TEXs[Pic1]);
	//printf("programId----:%d\n", programId);
	//glUniform1i(glGetUniformLocation(programId, "ourTexture1"), 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, TEXs[Pic2]);
	glUniform1i(glGetUniformLocation(programId, "ourTexture2"), 1);
}