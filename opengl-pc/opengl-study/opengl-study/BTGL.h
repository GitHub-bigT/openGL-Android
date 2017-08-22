#ifndef _BTGL_H_
#define _BTGL_H_
#include <GL\glew.h>
#include <iostream>
#include "File.h"

class BTVaoVbo{
public:
	void initVaoVbo();
	void drawArrays();
private:
	enum VAO_IDs{ TriangleAO, NumVaoIds };
	enum VBO_IDs{ TriangleBO, NumVBOIds };
	enum Attrib_IDs{vPosition = 1, vColor = 2};
	GLuint VAOs[NumVaoIds];
	GLuint VBOs[NumVaoIds];
	
};

class BTprogram {
public:
	GLuint loadShader(GLenum type, const char* shaderSource);
	void createProgram();
	void useProgram();
private:
	GLuint programId;
	GLuint v_shader_Id;
	GLuint f_shader_Id;
};

#endif