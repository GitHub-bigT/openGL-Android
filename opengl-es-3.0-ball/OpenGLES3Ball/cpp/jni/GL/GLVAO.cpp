#include "GL.h"

void GLVAO::setVAO(int width , int height){
	//VAO
	enum{VAOBall,NumVAOs};
	GLuint VAOs[NumVAOs];
	//VBO
	enum{VBOBall,NumVBOs};
	GLuint VBOs[NumVBOs];
	
	//generate with bind VAO VBO
	glGenVertexArrays(NumVAOs,VAOs);
	glGenBuffers(NumVBOs,VBOs);
	glBindVertexArray(VAOs[VAOBall]);
	glBindBuffer(GL_ARRAY_BUFFER,VBOs[VBOBall]);
	
	//check bind
	tools->checkIsBindVertexArray(VAOs[VAOBall]);
	tools->checkIsBindBuffer(VBOs[VBOBall]);
	
	//generate ball vertex coord
	generateBallVertexCoord(width,height);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData),vertexData,GL_STATIC_DRAW);
	
	//transmission data
	glVertexAttribPointer(vPosition,3,GL_FLOAT,GL_FALSE,0,0);
	//enable
	glEnableVertexAttribArray(vPosition);
}
void  GLVAO::generateBallVertexCoord(int width , int height){
	//LOGE("vao ::: %d,%d",width,height);
	/**
	* 球的坐标
	* x = R*cos(a)*sin(b)
	* y = R*sin(a)
	* z = R*cos(a)*cos(b)
	*/
	//宽高比例
	//float whProportion = width/height;
	//球的半径
	//float radius[3] = {1.0f , 1.0f * whProportion, 1.0f};
	//球坐标
	float x1,x2,y1,y2,z1,z2;
	//x z轴上的斜边(R*cos(a))
	GLfloat hypotenuse1 , hypotenuse2;
	//x y轴夹角的变换角度大小。值越小越像趋于球体
	int step = 30;
	/**画球的思想
	* 纬度方向 -90度 - 90度旋转出一个扇形
	* 经度方向旋转360度画出球
	* 这里采用的办法是每两条相邻的纬度画出一个体
	*/
	int n = 0 ;
	for(int angleA = -90 ; angleA<-60 ; angleA+=step){	
		//纬度方向
		//x z轴上的斜边(R*cos(a))
		hypotenuse1 = cos(M_PI * angleA / 180.0f);
		hypotenuse2 = cos(M_PI * (angleA + step) / 180.0f);
		y1 = sin(M_PI * angleA / 180.0f);
		y2 = sin(M_PI * (angleA + step) / 180.0f);
		for(int angleB = 0 ; angleB<360 ; angleB+=step){
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
			vertexData[n+1][0] = x2;
			vertexData[n+1][1] = y2;
			vertexData[n+1][2] = z2;
			n+=2;
		}
	}
	for(int i = 0 ; i < sizeof(vertexData)/(sizeof(float)*3);i++) {
		//LOGE("index %d , ball coord:x,y,z: %f,%f,%f",i,vertexData[i][0],vertexData[i][1],vertexData[i][2]);
	}
}