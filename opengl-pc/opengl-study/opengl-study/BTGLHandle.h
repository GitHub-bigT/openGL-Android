#ifndef _BTGLHANDLE_H_
#define _BTGLHANDLE_H_

#include <iostream>
#include <GL\glew.h>

class Handle
{
public:
	Handle();
	~Handle();
	//��һ��������
	void drawTriangles();
private:
	enum VAO_IDs{TriangleAO,NumVaoIds};
	enum VBO_IDs{TriangleBO,NumVBOIds};
	GLuint VAOs[NumVaoIds];
	GLuint VBOs[NumVaoIds];
};


#endif