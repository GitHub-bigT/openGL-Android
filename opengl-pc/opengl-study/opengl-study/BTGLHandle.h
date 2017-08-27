#ifndef _BTGLHANDLE_H_
#define _BTGLHANDLE_H_

#include <iostream>
#include <GL\glew.h>
#include "BTGL.h"

class Handle
{
public:
	Handle();
	~Handle();
	void init(int type);
	//画一个三角形
	void drawTriangles(int type, float alpha, float rotateAngle);
	//画一个球
	void drawBall();
private:
	BTVaoVbo* btVaoVbo;
	GLuint programId;
};


#endif