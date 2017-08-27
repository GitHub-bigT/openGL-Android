#include "BTGL.h"

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
	GLfloat triangleVertex[8] = {
		1.0f, 1.0f, //右上
		1.0f, -1.0f, //右下
		-1.0f, -1.0f, //左下
		-1.0f, 1.0f, //左上
	};
	GLushort triangleIndex[6] = {
			0,1,3,
			1,2,3
	};
	GLfloat triangleColor[12] = {
			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,
	};
	GLfloat triangleTexture[8] = {
			1.0f, 1.0f,
			1.0f, 0.0f,
			0.0f, 0.0f,
			0.0f, 1.0f
	};
	
	//图1 纹理
	int width1, height1, nrChannels1;
	unsigned char *image1 = stbi_load("./sImage/biaoqing2.jpg", &width1, &height1, &nrChannels1, 0);
	//unsigned char* image1 = SOIL_load_image("./sImage/biaoqing3.jpg", &width1, &height1, 0, SOIL_LOAD_RGB);

	printf("image1  ==== :%s\n", image1);
	printf("char[] length:=====%d\n", getLen(image1));
	glGenTextures(NumTexIds, TEXs);
	glBindTexture(GL_TEXTURE_2D, TEXs[Pic1]);
	//设置纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//设置纹理过滤
	//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	printf("width1  ==== :%d\n", width1);
	printf("height1  ==== :%d\n", height1);
	printf("nrChannels1  ==== :%d\n", nrChannels1);
	printf("image : -=================================\n");

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, image1);
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放图像的内存、解绑纹理
	//SOIL_free_image_data(image1);
	stbi_image_free(image1);
	glBindTexture(GL_TEXTURE_2D, 0);

	//图2 纹理
	int width2, height2, nrChannels2;
	unsigned char *image2 = stbi_load("./sImage/wx10.jpg", &width2, &height2, &nrChannels2, 0);
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

	printf("sizeof char --> %d \n" , sizeof(char));
	printf("sizeof unsigned char --> %d \n", sizeof(unsigned char));
	printf("sizeof char* --> %d \n", sizeof(char*));
	printf("sizeof unsigned char* --> %d \n", sizeof(unsigned char*));
	printf("sizeof int --> %d \n", sizeof(int));
	printf("sizeof short --> %d \n", sizeof(short));
	printf("sizeof float --> %d \n", sizeof(float));
	printf("sizeof double --> %d \n", sizeof(double));
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	//glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
	//glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	//glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image2);
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放图像内存、解绑纹理
	//SOIL_free_image_data(image2);
	stbi_image_free(image2);
	glBindTexture(GL_TEXTURE_2D,0);

	//1.绑定VBO
	glGenBuffers(NumVBOIds, VBOs);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[TriangleVBO]);
	//2.将数据传给显存当中的ARRAY_BUFFER缓冲块中
	glBufferData(GL_ARRAY_BUFFER,sizeof(triangleVertex)+sizeof(triangleColor)+sizeof(triangleTexture),NULL,GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(triangleVertex),triangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER,sizeof(triangleVertex), sizeof(triangleColor), triangleColor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(triangleVertex) + sizeof(triangleColor), sizeof(triangleTexture), triangleTexture);
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
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(sizeof(triangleVertex)+sizeof(triangleColor)));
	glEnableVertexAttribArray(vPosition);
	glEnableVertexAttribArray(vColor);
	glEnableVertexAttribArray(vTexCoord);
	//3.解绑vao,避免在其他地方错误的配置
	glBindVertexArray(0);
}

void BTVaoVbo::initBallVaoVbo(){
	//x y轴夹角的变换角度大小。值越小越像趋于球体
	int step = 3;
	int vertexCount = 14400;
	//顶点数据
	float vertexData[14400][3];
	//LOGE("vao ::: %d,%d",width,height);
	/**
	* 球的坐标
	* x = R*cos(a)*sin(b)
	* y = R*sin(a)
	* z = R*cos(a)*cos(b)
	*/
	//宽高比例
	//float whProportion = (float)width / (float)height;
	float whProportion = 800.0f / 600.0f;
	//球的半径
	//float radius[3] = { 1.0f, 1.0f * whProportion, 1.0f };
	float radius[3] = { 1.0f, 1.0f, 1.0f };
	//球坐标
	float x1, x2, y1, y2, z1, z2;
	//x z轴上的斜边(R*cos(a))
	GLfloat hypotenuse1, hypotenuse2;

	/**画球的思想
	* 纬度方向 -90度 - 90度旋转出一个扇形
	* 经度方向旋转360度画出球
	* 这里采用的办法是每两条相邻的纬度画出一个体
	*/
	int n = 0;
	for (int angleA = -90; angleA<90; angleA += step){
		//纬度方向
		//x z轴上的斜边(R*cos(a))
		hypotenuse1 = radius[0] * cos(M_PI * angleA / 180.0f);
		hypotenuse2 = radius[0] * cos(M_PI * (angleA + step) / 180.0f);
		y1 = radius[1] * sin(M_PI * angleA / 180.0f);
		y2 = radius[1] * sin(M_PI * (angleA + step) / 180.0f);
		for (int angleB = 0; angleB<360; angleB += step){
			//经度方向
			x1 = hypotenuse1 * sin(M_PI * angleB / 180.0f);
			x2 = hypotenuse2 * sin(M_PI * angleB / 180.0f);
			z1 = hypotenuse1 * cos(M_PI * angleB / 180.0f);
			z2 = hypotenuse2 * cos(M_PI * angleB / 180.0f);
			//LOGI("index:%d,latitude:%d,longitude:%d ,ball coord:x1,y1,z1: %f,%f,%f" , n,angleA,angleB,x1,y1,z1);
			//LOGI("index:%d,latitude:%d,longitude:%d ,ball coord:x2,y2,z2: %f,%f,%f" , n,angleA+step,angleB+step,x2,y2,z2);	
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

void BTVaoVbo::drawArrays(int type, GLuint programId, float alpha, float rotateAngle){
	//glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//三角形
	if (type == 1)
	{
		glBindVertexArray(VAOs[TriangleVAO]);
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, glm::radians(rotateAngle), glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "rotate"), 1, GL_FALSE, glm::value_ptr(rotate));

		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "scale"), 1, GL_FALSE, glm::value_ptr(scale));
		//GL_FILL 默认模式  
		//GL_LINE 线框模式
		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//glDrawArrays(GL_TRIANGLE_STRIP,0,4);
		glUniform1f(glGetUniformLocation(programId, "update_alpha"), alpha);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
	}
	//球
	if (type == 2)
	{
		glBindVertexArray(VAOs[BallVAO]);
		glm::mat4 rotate;
		rotate = glm::rotate(rotate, glm::radians(rotateAngle), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "rotate"), 1, GL_FALSE, glm::value_ptr(rotate));

		glm::mat4 scale;
		scale = glm::scale(scale, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(glGetUniformLocation(programId, "scale"), 1, GL_FALSE, glm::value_ptr(scale));
		glDrawArrays(GL_LINE_STRIP, 0, 14400);
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