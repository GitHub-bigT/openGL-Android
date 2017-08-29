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
	void drawTriangles(int type, float alpha, float rotateAngle, glm::vec3 cameraPos, glm::vec3 cameraFront, glm::vec3 cameraUp);
private:
	BTVaoVbo* btVaoVbo;
	GLuint programId;
};


#endif