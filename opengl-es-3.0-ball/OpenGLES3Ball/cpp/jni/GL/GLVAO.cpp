#include "GL.h"
#include <math.h>
void GLVAO::setVAO(int width , int height){
	LOGI("radians %f" , cos(20.0f));
	//LOGE("vao ::: %d,%d",width,height);
	/**
	* �������
	* x = R*cos(a)*sin(b)
	* y = R*sin(a)
	* z = R*cos(a)*cos(b)
	*/
	//��߱���
	//float whProportion = width/height;
	//��İ뾶
	//float radius[3] = {1.0f , 1.0f * whProportion, 1.0f};
	//y������
	float y1,y2;
	//x z���ϵ�б��(R*cos(a))
	GLfloat hypotenuse1 , hypotenuse2;
	//x z��нǵ�sin cos
	float sin,cos;
	//x y��нǵı任�Ƕȴ�С��ֵԽСԽ����������
	int step = 30;
	//��������
	float vertexData[24][3];
	//VAO
	enum{VAOBall,NumVAOs};
	float VAOs[NumVAOs];
	//VBO
	enum{VBOBall,NumVBOs};
	float VBOs[NumVBOs];
	for(int angleA = -90 ; angleA<-60 ; angleA+=step){
		GLfloat radians = M_PI * angleA / 180.0f;
		
		//x z���ϵ�б��(R*cos(a))
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