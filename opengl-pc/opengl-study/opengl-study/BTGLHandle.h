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
	void init();
	//��һ��������
	void drawTriangles();
private:
	BTVaoVbo* btVaoVbo;
	GLuint programId;
};


#endif