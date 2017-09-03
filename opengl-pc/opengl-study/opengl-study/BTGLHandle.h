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
	void drawTriangles(int type, float alpha, glm::mat4 viewMatrix , GLfloat aspect);
private:
	BTVaoVbo* btVaoVbo;
	GLuint programId;
};


#endif