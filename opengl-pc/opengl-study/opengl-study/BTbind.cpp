#include "BTGL.h"

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(2.0f, 5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f, 3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f, 2.0f, -2.5f),
	glm::vec3(1.5f, 0.2f, -1.5f),
	glm::vec3(-1.3f, 1.0f, -1.5f)
};

int getLen(const unsigned char s[])
{
	int nLen = 0;
	const unsigned char* p = s;
	while (*p != 0){
		nLen++;
		p++;
	}
	return nLen;
}

void BTVaoVbo::initVaoVbo(){
	//std::cout << "init vao vbo" << std::endl;
	GLfloat triangleVertex[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f
	};
	GLushort triangleIndex[6] = {
			0,1,3,
			1,2,3
	};
	
	//ͼ1 ����
	int width1, height1, nrChannels1;
	unsigned char *image1 = stbi_load("./sImage/container.jpg", &width1, &height1, &nrChannels1, 0);
	//unsigned char* image1 = SOIL_load_image("./sImage/biaoqing3.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);

	printf("image1  ==== :%s\n", image1);
	printf("char[] length:=====%d\n", getLen(image1));
	glGenTextures(NumTexIds, TEXs);
	glBindTexture(GL_TEXTURE_2D, TEXs[Pic1]);
	//���������Ʒ�ʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//�����������
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	printf("width1  ==== :%d\n", width1);
	printf("height1  ==== :%d\n", height1);
	printf("nrChannels1  ==== :%d\n", nrChannels1);
	printf("image : -=================================\n");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ����ڴ桢�������
	//SOIL_free_image_data(image1);
	stbi_image_free(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	//ͼ2 ����
	int width2, height2, nrChannels2;
	unsigned char *image2 = stbi_load("./sImage/awesomeface.png", &width2, &height2, &nrChannels2, 0);
	//unsigned char* image2 = SOIL_load_image("./sImage/wx4.jpg", &width2, &height2, 0 ,SOIL_LOAD_RGB);
	printf("char[] length:=====%ld\n", strlen((char *)image2));
	printf("size of >>>>> %ld\n", sizeof(image2));
	printf("width2  ==== :%d\n", width2);
	printf("height2  ==== :%d\n", height2);
	printf("nrChannels2  ==== :%d\n", nrChannels2);

	//unsigned char* image2 = SOIL_load_image("./sImage/biaoqing3.jpg", &width2, &height2, 0, SOIL_LOAD_RGB);
	//printf("image2  ==== :%s\n",image2);
	glBindTexture(GL_TEXTURE_2D,TEXs[Pic2]);
	GLfloat borderColor[] = {0.0f,0.0f,0.0f,1.0f};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width2, height2, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	//�ͷ�ͼ���ڴ桢�������
	//SOIL_free_image_data(image2);
	stbi_image_free(image2);
	glBindTexture(GL_TEXTURE_2D,0);

	//1.��VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//2.�����ݴ����Դ浱�е�ARRAY_BUFFER�������
	printf("size size size ///////  %d \n" , sizeof(triangleVertex));
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertex), triangleVertex, GL_STATIC_DRAW);

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
	//glGenBuffers(NumEBOIds , EBOs);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER , EBOs[TriangleEBO]);
	//GLboolean b2 = glIsBuffer(EBOs[TriangleEBO]);
	//printf("triangle ebo bind: %d\n", b2);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(triangleIndex),triangleIndex,GL_STATIC_DRAW);

	//2.���ö�������ָ��
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5 , 0);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid *)(sizeof(GLfloat) * 3));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vTexCoord);
	//3.���vao,�����������ط����������
	glBindVertexArray(0);
	//���������(z)
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 directionVector = glm::normalize(cameraPos - cameraTarget);
	//����(x)(�������)
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 cameraRight = glm::normalize(glm::cross(up, directionVector));
	printf("cameraRight : x:%f,y:%f,z:%f\n", cameraRight.x, cameraRight.y, cameraRight.z);
	//����
	glm::vec3 cameraUp = glm::normalize(glm::cross(cameraRight,directionVector));
}

void BTVaoVbo::initBallVaoVbo(){
	//��������
	float vertexData[VERTEX_COUNT][3];
	//LOGE("vao ::: %d,%d",width,height);
	/**
	* �������
	* x = R*cos(a)*sin(b)
	* y = R*sin(a)
	* z = R*cos(a)*cos(b)
	*/
	//��İ뾶
	float radius[3] = { 0.5f, 0.5f, 0.5f };
	//������
	float x1, x2, y1, y2, z1, z2;
	//x z���ϵ�б��(R*cos(a))
	GLfloat hypotenuse1, hypotenuse2;

	/**�����˼��
	* γ�ȷ��� -90�� - 90����ת��һ������
	* ���ȷ�����ת360�Ȼ�����
	* ������õİ취��ÿ�������ڵ�γ�Ȼ���һ����
	*/
	int n = 0;
	for (int angleA = -90; angleA<90; angleA += STEP){
		//γ�ȷ���
		//x z���ϵ�б��(R*cos(a))
		hypotenuse1 = radius[0] * cos(M_PI * angleA / 180.0f);
		hypotenuse2 = radius[0] * cos(M_PI * (angleA + STEP) / 180.0f);
		y1 = radius[1] * sin(M_PI * angleA / 180.0f);
		y2 = radius[1] * sin(M_PI * (angleA + STEP) / 180.0f);
		for (int angleB = 0; angleB<360; angleB += STEP){
			//���ȷ���
			x1 = hypotenuse1 * sin(M_PI * angleB / 180.0f);
			x2 = hypotenuse2 * sin(M_PI * angleB / 180.0f);
			z1 = hypotenuse1 * cos(M_PI * angleB / 180.0f);
			z2 = hypotenuse2 * cos(M_PI * angleB / 180.0f);
			printf("index:%d,latitude:%d,longitude:%d ,ball coord:x1,y1,z1: %f,%f,%f\n" , n,angleA , angleB,x1,y1,z1);
			printf("index:%d,latitude:%d,longitude:%d ,ball coord:x2,y2,z2: %f,%f,%f\n", n, angleA + STEP, angleB + STEP, x2, y2, z2);
			vertexData[n][0] = x1;
			vertexData[n][1] = y1;
			vertexData[n][2] = z1;
			vertexData[n + 1][0] = x2;
			vertexData[n + 1][1] = y2;
			vertexData[n + 1][2] = z2;
			n += 2;
		}
	}
	glGenVertexArrays(NumVaoIds, VAOs);
	glGenBuffers(NumVBOIds, VBOs);

	glBindVertexArray(VAOs[BallVAO]);
	GLboolean b1 = glIsVertexArray(VAOs[BallVAO]);
	printf("ball vao bind: %d\n", b1);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[BallVAO]);
	GLboolean b2 = glIsBuffer(VBOs[BallVAO]);
	printf("ball vbo bind: %d\n", b2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	//transmission data
	glVertexAttribPointer(vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//enable
	glEnableVertexAttribArray(vPosition);
}

void BTVaoVbo::drawArrays(int type, GLuint programId, float alpha, float rotateAngle, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp){
	glEnable(GL_DEPTH_TEST);

	glClearColor(0.5f,0.5f,0.5f,1.0f);
	//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//������
	if (type == 1)
	{
		glBindVertexArray(VAOs[TriangleVAO]);
		//model = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//��ͼ����
		glm::mat4 view;
		//view = glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));
		
		//look at λ�� Ŀ�� ������
		GLfloat radius = 10.0f;
		GLfloat camX = sin(glfwGetTime()) * radius;
		GLfloat camZ = cos(glfwGetTime()) * radius;
		view = glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
		glUniformMatrix4fv(glGetUniformLocation(programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//ͶӰ����
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f),800.0f / 600.0f , 0.01f,100.0f);
		glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//GL_FILL Ĭ��ģʽ  
		//GL_LINE �߿�ģʽ
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
		glUniform1f(glGetUniformLocation(programId, "update_alpha"), alpha);
		for (GLint i = 0; i < 10; i++)
		{
			//ģ�;���
			glm::mat4 model;
			//���� * ��ת * ƽ��
			model = glm::translate(model, cubePositions[i]);
			GLfloat angle = 20.0f * i;
			if (i>=0 && i<3){
				model = glm::rotate(model, glm::radians(30.0f) * (GLfloat) glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.2f));
			}
			else
			{
				model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.2f));
			}
			glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
	}
	//��
	if (type == 2)
	{
		glBindVertexArray(VAOs[BallVAO]);
		//ģ�;���
		glm::mat4 model;
		model = glm::rotate(model, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
		//��ͼ����
		glm::mat4 view;
		view = glm::translate(view,glm::vec3(0.0f,0.0f,-3.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId,"view"),1,GL_FALSE,glm::value_ptr(view));
		//ͶӰ����
		glm::mat4 projection;
			//͸��ͶӰ	
		projection = glm::perspective(45.0f , 800.0f / 600.0f , 0.1f , 100.0f);
			//����ͶӰ
		//projection = glm::ortho(0.0f,800.0f,0.0f,600.0f,0.1f,100.0f);
		glUniformMatrix4fv(glGetUniformLocation(programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glDrawArrays(GL_LINE_STRIP, 0, VERTEX_COUNT);
	}
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