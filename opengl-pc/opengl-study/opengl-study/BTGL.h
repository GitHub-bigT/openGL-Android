#ifndef _BTGL_H_
#define _BTGL_H_
#include <GL\glew.h>
#include <iostream>
#include "File.h"
#include <SOIL.h>

class BTVaoVbo{
public:
	void initVaoVbo();
	void drawArrays();
private:
	enum VAO_IDs{ TriangleVAO, NumVaoIds };
	enum VBO_IDs{ TriangleVBO, NumVBOIds };
	enum EBO_IDs{ TriangleEBO, NumEBOIds };
	enum Attrib_IDs{ vPosition = 1, vColor = 2, vTexCoord = 3};
	GLuint VAOs[NumVaoIds];
	GLuint VBOs[NumVaoIds];
	GLuint EBOs[NumEBOIds];
	GLuint texture;
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