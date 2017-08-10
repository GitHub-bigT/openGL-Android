#include "GL.h"
#include <math.h>
void GLVAO::setVAO(int width , int height){
	LOGI("radians %f" , cos(20.0f));
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
	//y轴坐标
	float y1,y2;
	//x z轴上的斜边(R*cos(a))
	GLfloat hypotenuse1 , hypotenuse2;
	//x z轴夹角的sin cos
	float sin,cos;
	//x y轴夹角的变换角度大小。值越小越像趋于球体
	int step = 30;
	//顶点数据
	float vertexData[24][3];
	//VAO
	enum{VAOBall,NumVAOs};
	float VAOs[NumVAOs];
	//VBO
	enum{VBOBall,NumVBOs};
	float VBOs[NumVBOs];
	for(int angleA = -90 ; angleA<-60 ; angleA+=step){
		GLfloat radians = M_PI * angleA / 180.0f;
		
		//x z轴上的斜边(R*cos(a))
		//hypotenuse1 = cos(radians);
		//hypotenuse2 = cos(M_PI * (angleA + step) / 180.0f);
		//y1 = sin(M_PI * angleA / 180.0f);
		//y2 = sin(M_PI * (angleA + step) / 180.0f);
		//LOGI("hypotenuse1: %f" , hypotenuse1);
		//LOGI("hypotenuse2: %f" , hypotenuse2);
		//LOGI("y1: %f" , y1);
		//LOGI("y2: %f" , y2);
	}
}